#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

static void
print_element_names(xmlNode * a_node)
{
    xmlNode *cur_node = NULL;

    for (cur_node = a_node; cur_node; cur_node = cur_node->next) {
        if (cur_node->type == XML_ELEMENT_NODE) {
            printf("node type: Element, name: %s\n", cur_node->name);
        }

        print_element_names(cur_node->children);
    }
}

// gcc main.c -I include -L  lib -llibxml2
int main() {
	const char *content = "<docs><doc></doc></docs>";
	xmlDocPtr doc; /* the resulting document tree */

	 /*
     * The document being in memory, it have no base per RFC 2396,
     * and the "noname.xml" argument will serve as its base.
     */
    doc = xmlReadMemory(content, strlen(content), "noname.xml", NULL, 0);
    if (doc == NULL) {
        fprintf(stderr, "Failed to parse document\n");
		return 1;
    }
	xmlNode *root_element = NULL;
	root_element = xmlDocGetRootElement(doc);
	print_element_names(root_element);
    xmlFreeDoc(doc);
	return 0;
}
