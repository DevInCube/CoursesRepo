#pragma once

#include <stdio.h>

typedef struct tree_s tree_t;

typedef void (*traversal_callback_fn)(void * userData, void * nodeData);
typedef void * (*data_constructor_fn)(void * fileTreeNode);
typedef void (*data_destructor_fn)(void * nodeData);

// open file and parse it's contents. Build a tree of students
tree_t * tree_new_from_stream(
		FILE * inputStream, 
		data_constructor_fn constructorCallback, 
		data_destructor_fn destructorCallback
	);
// destructor
void tree_free(tree_t * self);
// tree nodes traversal with callback function and userdata to pass to each callback call
void tree_traversal(tree_t * self, void * userData, traversal_callback_fn callback);
