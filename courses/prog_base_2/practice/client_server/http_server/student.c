#include <stdlib.h>
#include <stdio.h>

#include "cJSON.h"

#include "student.h"

static cJSON * _student_serializeJSON(const student_t * self) {
    cJSON * jStudent = cJSON_CreateObject();
	cJSON_AddItemToObject(jStudent, "name", cJSON_CreateString(self->name));
	cJSON_AddItemToObject(jStudent, "surname", cJSON_CreateString(self->surname));
	cJSON_AddItemToObject(jStudent, "year", cJSON_CreateNumber(self->year));
	cJSON_AddItemToObject(jStudent, "score", cJSON_CreateNumber(self->score));
	cJSON * jGroup = cJSON_CreateObject();
	cJSON_AddItemToObject(jGroup, "name", cJSON_CreateString(self->group.name));
	cJSON_AddItemToObject(jGroup, "faculty", cJSON_CreateString(self->group.faculty));
	cJSON_AddItemToObject(jStudent, "group", jGroup);
	return jStudent;
}

student_t
student_empty(void) {
    student_t st = {
        .name = "",
        .surname = "",
        .year = 0,
        .score = 0.0,
        .group = {
            .name = "",
            .faculty = ""
        }
    };
    return st;
}

const char *
student_toJSON(const student_t * self) {
    cJSON * jStudent = _student_serializeJSON(self);
	const char * jsonString = cJSON_Print(jStudent);
	cJSON_Delete(jStudent);
    return jsonString;
}

const char *
student_listToJSON(const student_t * const studentList, int studentListSize) {
    cJSON * jArr = cJSON_CreateArray();
    for (int i = 0; i < studentListSize; i++) {
        cJSON * jStudent = _student_serializeJSON(studentList + i);
        cJSON_AddItemToArray(jArr, jStudent);
    }
    const char * jsonString = cJSON_Print(jArr);
	cJSON_Delete(jArr);
    return jsonString;
}

