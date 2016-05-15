#include "tree.h"

typedef struct node_s node_t;

struct node_s {
	void * data;
	node_t * left;
	node_t * right;
};

struct tree_s {
	node_t * root;
};

tree_t * tree_new_from_stream(FILE * inputStream, data_constructor_fn callback) {
	/* */
}

void tree_free(tree_t * self) {
	/* */
}

void tree_traversal(tree_t * self, void * userdata, traversal_callback_fn callback) {
	/* */
}
