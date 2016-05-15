#include <stdlib.h>
#include <stdio.h>

#include "tree.h"

/* Student */
typedef struct student_s student_t;

struct student_s {
	char fullname[50];
	int year;
	double score;
};

// write students data to stream
void student_write(student_t * self, FILE * outputStream) {
	/* */
}

student_t * student_new_from_json_object(void * fileTreeNode) {
	// cJSON * jObject = (cJSON *)fileTreeNode;
	/* */
}

void * student_free(student_t * self) {
	/* */
}

// traversal filter output
void print_highscored(void * userdata, student_t * student) {
	/* */
}

// traversal search
void find_eldest(void * userdata, student_t * student) {
	/* */
}

int main(void) {
	FILE * inputStream = NULL;
	// @todo open file stream
	tree_t * tree = tree_new_from_stream(
			inputStream, 
			(data_constructor_fn) student_new_from_json_object,
			(data_destructor_fn) student_free
		);
	// @todo close file stream
	if (NULL == tree) {
		/* */
	}
	void * userdata = NULL; // value can be changed
	//
	// output students by a specified condition
	tree_traversal(tree, NULL, (traversal_callback_fn)print_highscored);
	//
	// find a student by a specified criteria
	tree_traversal(tree, userdata, (traversal_callback_fn)find_eldest);
	//
	// output student data to console if search was successfull
	// @todo student_write()
	tree_free(tree);
	return 0;
}
