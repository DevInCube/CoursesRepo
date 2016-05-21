#ifndef LIST_IT_H_INCLUDED
#define LIST_IT_H_INCLUDED

typedef void (*iteration_fn)(void * value, int index, list_t * list, void * context);
typedef int (*predicate_fn)(void * value, int index, list_t * list, void * context);
typedef void * (*map_fn)(void * value, int index, list_t * list, void * context);
typedef void * (*reduce_fn)(void * prevValue, void * value, int index, list_t * list, void * context);

/**
*   list element index-value pair
*/
typedef struct list_entry_s list_entry_t;

/**
*   list iterator type
*/
typedef struct list_it_s list_it_t;

/**
*   list iterator element
*/
typedef struct list_it_elem_s list_it_elem_t;

/**
*   get next list iterator element
*/
list_it_elem_t * list_it_next(list_it_t * self);
void list_it_free(list_it_t * self);

/**
*   @brief Calls callback on for each element in array
*/
void list_forEach(list_t * self, iteration_fn callback, void * context);

/**
*   @return new list_it_t object that contains the key\value pair for each index in the list
*
*   do list_it_free() after usage of list_t pointer
*/
list_it_t * list_entries(list_t * self);

/**
*   @return true if every element in this array satisfies
*   the provided testing callback function
*
*   Stops iterating when testing function returns first false value
*/
int list_every(list_t * self, predicate_fn callback, void * context);

/**
*   @return true if at least one element in this array
*   satisfies the provided testing callback function
*
*   Stops iterating when testing function returns first true value
*/
int list_some(list_t * self, predicate_fn callback, void * context);

/**
*   @return new list with all the elements of this array for
*   which the provided filtering callback function returns true
*/
list_t * list_filter(list_t * self, predicate_fn callback, void * context);

/**
*   @return new list with the results of calling a provided
*   function on every element in this array
*/
list_t * list_map(list_t * self, map_fn callback, void * context);

/**
*   @brief  Apply a callback function agains an accumulator and each
*   value of the list (from left-to-right) as to reduce it to a single value
*   @param initial - initial accumulator value
*   @return accumulated value
*/
void * list_reduce(list_t * self, reduce_fn callback, void * context, void * initial);

/**
*   @brief  Apply a callback function agains an accumulator and each
*   value of the list (from right-to-left) as to reduce it to a single value
*   @param initial - initial accumulator value
*   @return accumulated value
*/
void * list_reduceRight(list_t * self, reduce_fn callback, void * context, void * initial);

/**
*   @return the found value in list, if an element in the list satisfies
*   the provided testing callback function of null if not found
*/
void * list_find(list_t * self, predicate_fn callback, void * context);

/**
*   @return the found index in the lis, if an element in the list satisfies
*   the provided testing callback function or -1 if not found
*/
int list_findIndex(list_t * self, predicate_fn callback, void * context);

struct list_entry_s {
    int index;
    void * value;
};

struct list_it_elem_s {
    int done;
    list_entry_t * entry;
};

#endif // LIST_IT_H_INCLUDED
