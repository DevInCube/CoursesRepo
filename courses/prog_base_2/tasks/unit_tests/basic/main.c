#include <stdlib.h>  // !
#include <stdarg.h>  // !
#include <stddef.h>  // !
#include <setjmp.h>  // !

#include <cmocka.h>

#include "stack.h"

// unit-test function
static void new_void_zeroCount(void **state)
{
    stack_t * stack = stack_new();
    assert_int_equal(stack_getCount(stack), 0);
    stack_free(stack);
}

static void push_oneValue_countOne(void **state)
{
    stack_t * stack = stack_new();
    stack_push(stack, 15);
    assert_int_equal(stack_getCount(stack), 1);
    stack_free(stack);
}

static void push_twoValues_countTwo(void **state)
{
    stack_t * stack = stack_new();
    stack_push(stack, 15);
    stack_push(stack, -15);
    assert_int_equal(stack_getCount(stack), 2);
    stack_free(stack);
}

static void pop_hasOnePushedValue_returnsPushedValueAndCountZero(void **state)
{
    stack_t * stack = stack_new();
    int theValue = 13;
    stack_push(stack, theValue);
    int popValue = stack_pop(stack);
    assert_int_equal(popValue, theValue);
    assert_int_equal(stack_getCount(stack), 0);
    stack_free(stack);
}

static void peek_hasOnePushedValue_returnsPushedValueAndCountOne(void **state)
{
    stack_t * stack = stack_new();
    int theValue = 13;
    stack_push(stack, theValue);
    assert_int_equal(stack_peek(stack), theValue);
    assert_int_equal(stack_getCount(stack), 1);
    stack_free(stack);
}

int main(void) {
    const struct CMUnitTest tests[] =
    {
        cmocka_unit_test(new_void_zeroCount),
        cmocka_unit_test(push_oneValue_countOne),
        cmocka_unit_test(push_twoValues_countTwo),
        cmocka_unit_test(pop_hasOnePushedValue_returnsPushedValueAndCountZero),
        cmocka_unit_test(peek_hasOnePushedValue_returnsPushedValueAndCountOne),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
