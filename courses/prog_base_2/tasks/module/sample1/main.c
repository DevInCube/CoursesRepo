#include <stdlib.h>
#include <stdio.h>

#include "array.h"

int main(void) {
    const int size = 5;
    array_t * arr = array_new(size);
    //arr->size = 3;
    array_fill(arr);
    array_setValueAt(arr, 2, 10);
    array_print(arr);
    printf("\nSize is %i\n", array_getSize(arr));
    array_free(arr);
    return 0;
}

