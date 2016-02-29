#include <stdio.h>
#include <stdlib.h>

#include "stack.h"

int main(void) {
    stack_t * st1 = stack_new();
    printf("Stack size is %i\n", stack_getCount(st1));
    int pushVal = 13;
    printf("Do push %i\n", pushVal);
    stack_push(st1, pushVal);
    printf("Stack size is %i\n", stack_getCount(st1));
    printf("Do pop top value\n");
    int popVal = stack_pop(st1);
    printf("Stack pop value is %i\n", popVal);
    printf("Stack size is %i\n", stack_getCount(st1));
    stack_free(st1);
    return 0;
}
