#include <stdio.h>
#include <stdlib.h>

#include "list.h"

static DEFINE_ALLOCATOR(int);

typedef struct test_s {
    int * val;
} test_t;

static void test_t_alloc(test_t * self) {
    self->val = malloc(sizeof(int) * 20);
    printf("test_t alloc()\n");
}
static DEFINE_ALLOCATOR_EX(test_t, test_t_alloc);
static void test_t_dealloc(void * val) {
    printf("test_t dealloc()\n");
    test_t * t = (test_t *)val;
    free(t->val);
    free(t);
}

int main(void) {
    list_t * l = list_new();
    list_insert(l, 0, int_allocate(5));
    list_insert(l, 0, int_allocate(7));
    list_insert(l, 0, int_allocate(-14));
    list_free(l);

    list_t * sList = list_new_dealloc(test_t_dealloc);
    list_insert(sList, 0, test_t_allocate((test_t){}));
    list_insert(sList, 0, test_t_allocate((test_t){}));
    list_insert(sList, 0, test_t_allocate((test_t){ .val = NULL }));
    list_free(sList);
    return 0;
}
