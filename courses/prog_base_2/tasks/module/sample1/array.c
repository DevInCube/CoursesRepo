#include <stdlib.h>
#include <stdio.h>
#include "array.h"

struct array_s {
    int * array;
    int size;
};

array_t * array_new(int size) {
    array_t * arr = malloc(sizeof(struct array_s));
    arr->array = malloc(sizeof(int) * size);
    arr->size = size;
    return arr;
}

void array_free(array_t * self) {
    free(self->array);
    free(self);
}

int array_getSize(const array_t * self) {
    return self->size;
}

int array_getValueAt(const array_t * self, int pos) {
    return self->array[pos];
}

void array_setValueAt(array_t * self, int pos, int val) {
    self->array[pos] = val;
}

void array_fill(array_t * self) {
    for (int i = 0; i < self->size; i++) {
        self->array[i] = i;
    }
}

void array_print(const array_t * self) {
    for (int i = 0; i < self->size; i++) {
        printf("%i, ", self->array[i]);
    }
}
