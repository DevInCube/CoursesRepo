#include <stdlib.h>

#include "list.h"

// LIST NODE
typedef struct node_s node_t;

struct node_s {
    node_t * next;
    node_t * prev;
    void * data;
};

static node_t * node_new(void * data);
static void node_free(node_t * self);
// END LIST NODE

static void _dealloc(list_t * self, void * value);

struct list_s {
    node_t * head;
    node_t * tail;
    int size;
    list_dealloc_fn deallocator;
};

// PUBLIC

list_t * list_new(void) {
    return list_new_dealloc(NULL);
}

list_t * list_new_copy(list_t * original) {
    list_t * self = list_new();
    for (int i = 0; i < list_getSize(original); i++) {
        list_push_back(self, list_get(original, i));
    }
    return self;
}

list_t * list_new_dealloc(list_dealloc_fn deallocator) {
    list_t * self = malloc(sizeof(struct list_s));
    self->deallocator = deallocator;
    self->size = 0;
    self->head = node_new(NULL);
    self->tail = node_new(NULL);
    self->head->next = self->tail;
    self->tail->prev = self->head;
    return self;
}

void list_free(list_t * self) {
    list_clear(self);
    free(self);
}

void list_push_back(list_t * self, void * data) {
    node_t * newNode = node_new(data);
    newNode->prev = self->tail->prev;
    newNode->prev->next = newNode;
    self->tail->prev = newNode;
    newNode->next = self->tail;
    self->size++;
}

void list_insert(list_t * self, int index, void * value) {
    if (index < 0 || index > self->size) {
        return;
    }
    if (index == self->size) {
        list_push_back(self, value);
        return;
    }
    node_t * cur = self->head;
    for (int i = 0; (cur->next != self->tail) && (i != index); i++) {
        cur = cur->next;
    }
    node_t * newNode = node_new(value);
    newNode->next = cur->next;
    newNode->next->prev = newNode;
    cur->next = newNode;
    newNode->prev = cur;
    self->size++;
}

void * list_pop_back(list_t * self) {
    if (self->size == 0) {
        return NULL;
    }
    node_t * delNode = self->tail->prev;
    delNode->prev->next = self->tail;
    self->tail->prev = delNode->prev;
    void * val = delNode->data;
    node_free(delNode);
    self->size--;
    return val;
}

int list_findIndex(list_t * self, void * data) {
    int i = 0;
    for (node_t * cur = self->head->next; cur != self->tail; cur = cur->next) {
        if (cur->data == data) {
            return i;
        }
        i++;
    }
    return -1;
}

void * list_remove(list_t * self, int index) {
    if (index < 0 || index >= self->size) {
        return NULL;
    }
    void * res = NULL;
    node_t * cur = self->head;
    for (int i = 0; (cur->next != self->tail) && (i != index); i++) {
        cur = cur->next;
    }
    node_t * delNode = cur->next;
    res = delNode->data;
    cur->next->next->prev = cur;
    cur->next = cur->next->next;
    self->size--;
    node_free(delNode);
    return res;
}

void list_clear(list_t * self) {
    while (self->size > 0) {
        void * val = list_pop_back(self);
        _dealloc(self, val);
    }
}

void * list_get(list_t * self, int index) {
    if (index < 0 || index >= self->size) {
        return NULL;
    }
    node_t * cur = self->head;
    for (int i = 0; (cur->next != self->tail) && (i != index); i++) {
        cur = cur->next;
    }
    return cur->next->data;
}

int list_getSize(list_t * self) {
    return self->size;
}

// PRIVATE

static node_t * node_new(void * data) {
    node_t * self = malloc(sizeof(struct node_s));
    self->next = NULL;
    self->prev = NULL;
    self->data = data;
    return self;
}

static void node_free(node_t * self) {
    free(self);
}

static void _dealloc(list_t * self, void * value) {
    if (NULL != self->deallocator) {
        self->deallocator(value);
    }
}
