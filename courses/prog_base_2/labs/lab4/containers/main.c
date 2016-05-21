#include <stdio.h>
#include <stdlib.h>

#include "list.h"
#include "list_it.h"

static DEFINE_PTR(int)

typedef struct test_s {
    int * val;
    int size;
} test_t;

static void test_t_alloc(test_t * self) {
    self->val = malloc(sizeof(int) * self->size);
    printf("test_t alloc(%i)\n", self->size);
}
static DEFINE_PTR_ALLOCATOR(test_t, test_t_alloc)
static void test_t_dealloc(void * val) {
    test_t * t = (test_t *)val;
    free(t->val);
    free(t);
    printf("test_t dealloc(%i)\n", t->size);
}

void print_element_cb(void * value, int index, list_t * list, void * context) {
    test_t * val = (test_t *)value;
    printf("forEach cb > test_t { .size = %i } at index %i\n", val->size, index);
}

int main(void) {
    list_t * l = list_new();
    list_insert(l, 0, int_ptr(5));
    list_insert(l, 0, int_ptr(7));
    list_insert(l, 0, int_ptr(-14));
    list_free(l);

    list_t * sList = list_new_dealloc(test_t_dealloc);
    list_insert(sList, 0, test_t_ptr((test_t){ .size = 10 }));
    list_insert(sList, 0, test_t_ptr((test_t){ .size = 20 }));
    list_insert(sList, 0, test_t_ptr((test_t){ .size = 30 }));

    list_forEach(sList, print_element_cb, NULL);

    list_free(sList);
    return 0;
}
