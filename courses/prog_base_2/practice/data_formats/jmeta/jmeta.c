#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "cJSON.h"

#include "jmeta.h"

static void jmeta_class_print_lvl(jmeta_class_t metaClass, const char * keyname, int offset, int lvl) {
	const int spaces = 4;
	char * pad = malloc(sizeof(char) * lvl * spaces + 1);
	for (int i = 0; i < lvl * spaces; i++) { pad[i] = ' '; };
	pad[lvl * spaces] = '\0';
	printf(pad);
	printf("%-10s %-10s", metaClass.name, keyname);
	if (0 != offset) {
		printf(" __(%i)", offset);
	}
	printf("\n");
	for (int i = 0; i < metaClass.metaSize; i++) {
		const jmeta_t meta = metaClass.meta[i];	
		if (JOBJECT == meta.type) {
			jmeta_class_print_lvl(*(meta.jmetaClass), meta.key, meta.offset, lvl + 1);
		} else if (JARRAY == meta.type) {
			
		} else {
			printf(pad);
			printf("    %-10s %-10s __(%i)\n", 
				jtype_toString(meta.type), meta.key, meta.offset);
		}
	}
	free(pad);
}

void jmeta_class_print(jmeta_class_t metaClass) {
	jmeta_class_print_lvl(metaClass, "", 0, 0);
}

cJSON * _jserialize(void * obj, jmeta_class_t metaClass) {
	cJSON * j = cJSON_CreateObject();
	for (int i = 0; i < metaClass.metaSize; i++) {
		const jmeta_t meta = metaClass.meta[i];
		void * ptr = JMETA_OFFSET(obj, meta.offset);
		cJSON * jitem = NULL;
		if (JOBJECT == meta.type) {
			jitem = _jserialize(ptr, *meta.jmetaClass);
		} else if (JARRAY == meta.type) {
			jitem = cJSON_CreateArray();
			for (int j = 0; j < meta.jArrMaxSize; j++) {
				jmeta_class_t itemMetaClass = *meta.jmetaClass;
				void * elemptr = JMETA_OFFSET(ptr, j * itemMetaClass.size);
				cJSON * jelem = _jserialize(elemptr, itemMetaClass);
				cJSON_AddItemToArray(jitem, jelem);
			}
		} else {
			if (JSTRING == meta.type) {
				char ** sptr = (char **)ptr;
				jitem = cJSON_CreateString(*sptr);
			} else if (JINTEGER == meta.type) {
				int * intptr = (int *)ptr;
				jitem = cJSON_CreateNumber((double)*intptr);
			} else if (JDOUBLE == meta.type) {
				double * dblptr = (double *)ptr;
				jitem = cJSON_CreateNumber(*dblptr);
			} else if (JBOOLEAN == meta.type) {
				int * boolptr = (int *)ptr;
				jitem = cJSON_CreateBool(*boolptr);
			}
		}
		if (NULL != jitem) {
			cJSON_AddItemToObject(j, meta.key, jitem);
		}
	}
	return j;
}

const char * jserialize(void * obj, jmeta_class_t metaClass) {
	cJSON * j = _jserialize(obj, metaClass);
	if (NULL == j) {
		return NULL; // ERROR
	}
	const char * res = cJSON_Print(j);
	cJSON_Delete(j);
	return res;
}

int _jfill(void * obj, jmeta_class_t metaClass, cJSON * j) {
	for (int i = 0; i < metaClass.metaSize; i++) {
		const jmeta_t meta = metaClass.meta[i];
		// @todo cJSON_HasObjectItem
		cJSON * jprop = cJSON_GetObjectItem(j, meta.key);
		void * ptr = JMETA_OFFSET(obj, meta.offset);
		if (JOBJECT == meta.type) {
			_jfill(ptr, *(meta.jmetaClass), jprop);
		} else if (JARRAY == meta.type) {
			// @todo implement
		} else {
			if (JSTRING == meta.type) {
				char ** sptr = (char **)ptr;
				*sptr = malloc(strlen(jprop->valuestring) + 1);
				strcpy(*sptr, jprop->valuestring);
			} else if (JINTEGER == meta.type) {
				int * intptr = (int *)ptr;
				*intptr = jprop->valueint;
			} else if (JDOUBLE == meta.type) {
				double * dblptr = (double *)ptr;
				*dblptr = jprop->valuedouble;
			} else if (JBOOLEAN == meta.type) {
				int * boolptr = (int *)ptr;
				*boolptr = jprop->valueint;
			} else {
				return 1; // ERROR
			}
		}
	}
	return 0; // OK
}

int jfill(void * obj, jmeta_class_t metaClass, const char * jstr) {
	//jmeta_class_print(metaClass);  // @test REMOVE
	cJSON * json = cJSON_Parse(jstr);
	int rc = _jfill(obj, metaClass, json);
	cJSON_Delete(json);
	return rc;
}


const char * jtype_toString(jtype type) {
	switch (type) {
		case JSTRING: 	return "JSTRING";
		case JINTEGER: 	return "JINTEGER";
		case JDOUBLE: 	return "JDOUBLE";
		case JBOOLEAN:	return "JBOOLEAN";
		case JARRAY: 	return "JARRAY";
		case JOBJECT: 	return "JOBJECT";
		default: 		return "(undefined)";
	}
}
