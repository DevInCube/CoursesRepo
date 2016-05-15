#ifndef LIST_H_INCLUDED
#define LIST_H_INCLUDED

#include <stdlib.h>

#define DEFINE_PTR(TYPE)              \
    void * TYPE ## _ptr(TYPE value) {       \
        TYPE * ptr = malloc(sizeof(TYPE));  \
        *ptr = value;                       \
        return ptr;                         \
    }

#define DEFINE_PTR_ALLOCATOR(TYPE, ALLOCATOR)\
    void * TYPE ## _ptr(TYPE value) {       \
        TYPE * ptr = malloc(sizeof(TYPE));  \
        *ptr = value;                       \
        ALLOCATOR(ptr);                     \
        return ptr;                         \
    }

typedef struct list_s list_t;
typedef void (* list_dealloc_fn)(void *);

list_t * list_new(void);
list_t * list_new_copy(list_t * original);
list_t * list_new_dealloc(list_dealloc_fn deallocator);
void list_free(list_t * self);

void list_push_back(list_t * self, void * data);
void list_insert(list_t * self, int position, void * value);
void * list_pop_back(list_t * self);
void * list_remove(list_t * self, int index);
void list_clear(list_t * self);
void * list_get(list_t * self, int index);
int list_findIndex(list_t * self, void * data);
int list_getSize(list_t * self);

#endif // LIST_H_INCLUDED
