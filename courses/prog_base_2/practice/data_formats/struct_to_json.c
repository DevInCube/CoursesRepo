#include <stdlib.h>
#include <stdio.h>

#include "cJSON.h"

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
	cJSON * jStudent = cJSON_CreateObject();
	cJSON_AddItemToObject(jStudent, "name", cJSON_CreateString(st.name));
	cJSON_AddItemToObject(jStudent, "surname", cJSON_CreateString(st.surname));
	cJSON_AddItemToObject(jStudent, "year", cJSON_CreateNumber(st.year));
	cJSON_AddItemToObject(jStudent, "score", cJSON_CreateNumber(st.score));
	cJSON * jGroup = cJSON_CreateObject();
	cJSON_AddItemToObject(jGroup, "name", cJSON_CreateString(st.group.name));
	cJSON_AddItemToObject(jGroup, "faculty", cJSON_CreateString(st.group.faculty));
	cJSON_AddItemToObject(jStudent, "group", jGroup);
	char * jsonString = cJSON_Print(jStudent);
	printf("%s\n", jsonString);
	free(jsonString);
	cJSON_Delete(jStudent);
	return 0;
}
