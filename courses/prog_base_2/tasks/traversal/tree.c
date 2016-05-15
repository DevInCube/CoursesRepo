#include "tree.h"

typedef struct node_s node_t;

struct node_s {
	void * data;
	node_t * left;
	node_t * right;
};

struct tree_s {
	node_t * root;
	data_destructor_fn data_destructor;
};

tree_t * tree_new_from_stream(
		FILE * inputStream, 
		data_constructor_fn constructorCallback, 
		data_destructor_fn destructorCallback
	) {
	/* */
}

void tree_free(tree_t * self) {
	/* */
}

void tree_traversal(tree_t * self, void * userdata, traversal_callback_fn callback) {
	/* */
}
