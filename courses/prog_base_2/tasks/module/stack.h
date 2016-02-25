#ifndef STACK_H_INCLUDED
#define STACK_H_INCLUDED

#include <stdlib.h>

// main type
typedef struct stack_s stack_t;

// constructor
stack_t * stack_new(void);
// destructor
void stack_free(stack_t * self);

// get-function
size_t stack_getCount(stack_t * self);
// methods
void stack_push(stack_t * self, int val);
int stack_pop(stack_t * self);
int stack_peek(stack_t * self);

// module function call status
typedef enum stack_status_t {
    STACK_OK,       // 0
    STACK_OVERFLOW, // 1
    STACK_EMPTY
} stack_status_t;

// get last function call status for specified stack
stack_status_t stack_getStatus(stack_t * self);

#endif // STACK_H_INCLUDED
