#include "stack.h"

struct stack_s {
    int array[100];
    int count;
};

stack_t * stack_new(void) {
    stack_t * s = malloc(sizeof(struct stack_s));
    s->count = 0;
    return s;
}

void stack_free(stack_t * self) {
    free(self);
}

int stack_getCount(stack_t * self) {
    return self->count;
}

void stack_push(stack_t * self, int val) {
    self->array[self->count] = val;
    self->count += 1;
}

int stack_pop(stack_t * self) {
    if (self->count > 0) {
        int val = self->array[self->count - 1];
        self->count -= 1;
        return val;
    }
}

int stack_peek(stack_t * self) {
    if (self->count > 0) {
        int val = self->array[self->count - 1];
        return val;
    }
}

void stack_print(stack_t * self) {
    for(int i = 0; i < self->count; i++) {
        printf("%3i ", self->array[i]);
    }
}
