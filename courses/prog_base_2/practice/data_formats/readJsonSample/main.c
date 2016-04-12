#include <stdio.h>
#include <stdlib.h>

#include "cJSON.h"

void parse(char * text)
{
	cJSON * jList = cJSON_Parse(text);
	if (!jList) {
		printf("Error before: [%s]\n", cJSON_GetErrorPtr());
		return;
	}

    int count = cJSON_GetArraySize(jList);
    for (int i = 0; i < count; i++) {
        cJSON * jItem = cJSON_GetArrayItem(jList, i);
        char * name = cJSON_GetObjectItem(jItem, "name")->valuestring;
        char * surname = cJSON_GetObjectItem(jItem, "surname")->valuestring;
        char * birthdate = cJSON_GetObjectItem(jItem, "birthdate")->valuestring;
        int year = cJSON_GetObjectItem(jItem, "year")->valueint;
        double score = cJSON_GetObjectItem(jItem, "score")->valuedouble;
        cJSON * jGroup = cJSON_GetObjectItem(jItem, "group");
        char * groupName = cJSON_GetObjectItem(jGroup, "name")->valuestring;
        char * faculty = cJSON_GetObjectItem(jGroup, "faculty")->valuestring;

        printf("Name: \t%s\nSurnm: \t%s\nYear: \t%i\nScore:\t%f\nBirth:\t%s\n", name, surname, year, score, birthdate);
        printf("Group:\t%s (%s)\n\n", groupName, faculty);
    }
    cJSON_Delete(jList);

}

// gcc main.c cJSON.c -lm
int main() {
	FILE * fr = fopen("students.json", "r");
    char text[1000];
    char line[100];
    while(fgets(line, 100, fr) != NULL) {
        strcat(text, line);
    }
	fclose(fr);
    // printf ("%s\n", text);
	parse(text);
	return 0;
}
