#include <stdlib.h>

#include "list.h"
#include "list_it.h"

void list_forEach(list_t * self, iteration_fn callback, void * ctx) {
    if (NULL == callback) {
        return;
    }
    const int size = list_getSize(self);
    for (int index = 0; index < size; index++) {
        callback(list_get(self, index), index, self, ctx);
    }
}
