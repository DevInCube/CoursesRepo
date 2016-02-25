#include "stack.h"

#define MAX_STACK_SIZE 100

// main type implementation
struct stack_s {
    // PRIVATE fields
    int * arr;              // array of stack elements
    int top;                // index of top element, default: -1
    stack_status_t status;  // lask method call status, default: STACK_OK
};

// private function prototypes
static void _resetStatus(stack_t * self);

// PUBLIC functions implementations

stack_t * stack_new(void) {
    stack_t * self = malloc(sizeof(struct stack_s));
    self->arr = malloc(MAX_STACK_SIZE * sizeof(int));
    self->top = -1;
    self->status = STACK_OK;
    return self;
}

void stack_free(stack_t * self) {
    free(self->arr);
    free(self);
}

void stack_push(stack_t * self, int val) {
    _resetStatus(self);
    if (stack_getCount(self) >= MAX_STACK_SIZE) {
        self->status = STACK_OVERFLOW;
        return;
    }
    self->top += 1;
    self->arr[self->top] = val;
}

int stack_pop(stack_t * self) {
    _resetStatus(self);
    if (self->top > -1) {
        int val = stack_peek(self);
        self->top--;
        return val;
    } else {
        self->status = STACK_EMPTY;
    }
}

int stack_peek(stack_t * self) {
    _resetStatus(self);
    if (stack_getCount(self) == 0) {
        self->status = STACK_EMPTY;
    }
    return self->arr[self->top];
}

size_t stack_getCount(stack_t * self) {
    return self->top + 1;
}

stack_status_t stack_getStatus(stack_t * self) {
    return self->status;
}

// PRIVATE functions implementations

static void _resetStatus(stack_t * self) {
    self->status = STACK_OK;
}
