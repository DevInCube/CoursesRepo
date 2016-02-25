#include <stdio.h>
#include <stdlib.h>

#include <assert.h>
#include "stack.h"

int main(void) {

    stack_t * st1 = stack_new();
    assert(stack_getStatus(st1) == STACK_OK);
    assert(stack_getCount(st1) == 0);
    int val = stack_pop(st1);
    assert(stack_getStatus(st1) == STACK_EMPTY);
    stack_push(st1, 10);
    assert(stack_getStatus(st1) == STACK_OK);
    assert(stack_getCount(st1) == 1);
    assert(stack_peek(st1) == 10);
    val = stack_pop(st1);
    assert(10 == val);
    assert(stack_getCount(st1) == 0);
    stack_free(st1);

    printf("SUCCESS!\n");
    return 0;
}
