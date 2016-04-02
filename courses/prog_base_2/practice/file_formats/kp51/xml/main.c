#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <libxml/parser.h>
#include <libxml/tree.h>

// linux: -lxml2
int main(void) {
    char line[100];
    char text[1000] = "";
    FILE * fr = fopen("students.xml", "r");
    while(fgets(line, 100, fr)) {
        strcat(text, line);
    }
    // printf("%s", text);

    xmlDoc * xDoc;
    xDoc = xmlReadMemory(text, strlen(text), NULL, NULL, 0);
    if (NULL == xDoc) {
        printf("Error parsing xml from memory");
        return 1;
    }
    xmlNode * xRootEl;
    xRootEl = xmlDocGetRootElement(xDoc);
    for(xmlNode * xCur = xRootEl->children; NULL != xCur ; xCur = xCur->next) {
        if (XML_ELEMENT_NODE == xCur->type) {
            printf("Tag name <%s>\n", xCur->name);

            for(xmlNode * xJ = xCur->children; NULL != xJ ; xJ = xJ->next) {
                if (XML_ELEMENT_NODE == xJ->type) {
                    if(strcmp(xJ->name, "group") == 0) {
                        xmlNode * xGroup = xJ;
                        xmlNode * xFac = xGroup->children->next;
                        char * name = xmlGetProp(xGroup, "name");
                        char * fac = xmlNodeGetContent(xFac);
                        printf("\t%10s: %s (%s)\n", "group", name, fac);
                        continue;
                    }
                    const char * content = xmlNodeGetContent(xJ);
                    printf("\t%10s: %s\n", xJ->name, content);
                }
            }
        }
    }
    return 0;
}
