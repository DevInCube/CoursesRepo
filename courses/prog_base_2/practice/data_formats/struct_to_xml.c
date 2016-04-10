#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

typedef struct {
	char * name;
	char * faculty;
} group_t;

typedef struct {
	char * 	name;
	char * 	surname;
	int 	year;
	float	score; 
	group_t group;
} student_t;

int main() {
	student_t st = {
		.name = "Taras",
		.surname = "Kartoplia",
		.year = 1990,
		.score = 4.9,
		.group = {
			.name = "KP-51",
			.faculty = "PBF",
		}
	};

	xmlDoc * doc = NULL;
	xmlNode * rootNode = NULL;
	xmlNode * studentNode = NULL;
	xmlNode * groupNode = NULL;
	char strBuf[100];
	
	doc = xmlNewDoc("1.0");
	rootNode = xmlNewNode(NULL, "students");
	xmlDocSetRootElement(doc, rootNode);
	
	studentNode = xmlNewChild(rootNode, NULL, "student", NULL);
	xmlNewChild(studentNode, NULL, "name", st.name);
	xmlNewChild(studentNode, NULL, "surname", st.surname);
	sprintf(strBuf, "%i", st.year);
	xmlNewChild(studentNode, NULL, "year", strBuf);
	sprintf(strBuf, "%f", st.score);
	xmlNewChild(studentNode, NULL, "score", strBuf);
	groupNode = xmlNewChild(studentNode, NULL, "group", NULL);
	xmlNewProp(groupNode, "name", st.group.name);
	xmlNewChild(groupNode, NULL, "faculty", st.group.faculty);
	
	xmlBuffer * bufferPtr = xmlBufferCreate();
	xmlNodeDump(bufferPtr, NULL, (xmlNode *)doc, 0, 1);
	printf("%s", (const char *)bufferPtr->content);
	xmlBufferFree(bufferPtr);
	
      xmlFreeDoc(doc);
	xmlCleanupParser();
	return 0;
}
