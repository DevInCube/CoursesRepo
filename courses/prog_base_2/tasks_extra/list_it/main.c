#include <stdio.h>
#include <stdlib.h>

#include "list.h"
#include "list_it.h"

void print_element_cb(void * value, int index, list_t * list, void * context) {
    const char * val = (const char *) value;
    printf("forEach cb: value %s at index %i\n", val, index);
}

int main(void) {
    list_t * list = list_new();

    list_pushBack(list, "Roman");
    list_pushBack(list, "Daria");
    list_pushBack(list, "Nastya");
    list_pushBack(list, "Vlad");

    list_forEach(list, print_element_cb, NULL);

    list_free(list);
    return 0;
}
