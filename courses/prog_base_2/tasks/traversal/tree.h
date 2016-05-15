#pragma once

#include <stdio.h>

typedef struct tree_s tree_t;

typedef void (*traversal_callback_fn)(void * userData, void * nodeData);
typedef void * (*data_constructor_fn)(void * fileTreeNode);

// parse stream data t build a tree
tree_t * tree_new_from_stream(FILE * inputStream, data_constructor_fn callback);
// tree destructor
void tree_free(tree_t * self);
// tree nodes traversal with callback function and userdata to pass to each callback call
void tree_traversal(tree_t * self, void * userData, traversal_callback_fn callback);
