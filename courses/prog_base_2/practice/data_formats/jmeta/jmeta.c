#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "cJSON.h"

#include "jmeta.h"

static char * g_error = "";

static void * getPointedAddress(void * pointerAddress) {
	intptr_t * ptr = (intptr_t *)pointerAddress;
	return (void *)(*ptr);
}

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
			printf(pad);
			printf("    %-10s<%s> %-10s[] __(%i):\n", 
				jtype_toString(meta.type), meta.jmetaClass->name, meta.key, meta.offset);
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

static cJSON * _jserialize(void * obj, jmeta_class_t metaClass) {
	cJSON * j = cJSON_CreateObject();
	for (int i = 0; i < metaClass.metaSize; i++) {
		const jmeta_t meta = metaClass.meta[i];
		void * ptr = JMETA_OFFSET(obj, meta.offset);
		cJSON * jitem = NULL;
		if (JOBJECT == meta.type) {
			if (meta.isPointer) {
				void * pointed = getPointedAddress(ptr);
				if (NULL == pointed) {
					jitem = cJSON_CreateNull();
				} else {
					jitem = _jserialize(pointed, *meta.jmetaClass);
				}
			} else {
				jitem = _jserialize(ptr, *meta.jmetaClass);
			}
		} else if (JARRAY == meta.type) {
			jitem = cJSON_CreateArray();
			int count = meta.jArrMaxSize;
			void * arrstartptr = ptr;
			if (meta.isPointer) {
				int * sizeintptr = (int *)JMETA_OFFSET(obj, meta.arrLenFieldOffset);
				count = *sizeintptr;
				void * pointed = getPointedAddress(ptr);
				if (NULL == pointed) {
					// @todo
				}
				arrstartptr = pointed;
			}
			jmeta_class_t itemMetaClass = *meta.jmetaClass;
			// @todo itemMetaClass.isPointer
			for (int j = 0; j < count; j++) {
				void * elemptr = JMETA_OFFSET(arrstartptr, j * itemMetaClass.size);
				cJSON * jelem = _jserialize(elemptr, itemMetaClass);
				cJSON_AddItemToArray(jitem, jelem);
			}
		} if (JSTRING == meta.type) {
			if (meta.isPointer) {
				char ** sptr = (char **)ptr;
				jitem = cJSON_CreateString(*sptr);
			} else {
				jitem = cJSON_CreateString(ptr);
			}
		} else if (JINTEGER == meta.type) {
			int * intptr = (int *)ptr;
			jitem = cJSON_CreateNumber((double)*intptr);
		} else if (JDOUBLE == meta.type) {
			double * dblptr = (double *)ptr;
			jitem = cJSON_CreateNumber(*dblptr);
		} else if (JBOOLEAN == meta.type) {
			int * boolptr = (int *)ptr;
			jitem = cJSON_CreateBool(*boolptr);
		} else {
			// @todo undefined
		}
		if (NULL != jitem) {
			cJSON_AddItemToObject(j, meta.key, jitem);
		} else {
			// @todo some weird error
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

/* 
	@todo add deserialize modes
		IGNORE 	- all pointers are ignored
		STRICT	- try to access and fill objects by their pointer (but checks for NULL pointers)
		WEAK	- set NULL to object pointer if JSON has value 'null'
		MALLOC 	- allocate memory for object pointers, even if they points to something
			required PTR_length field in struct for arrays
	
	add STRICT or IGNORE errors modes
*/
static int _jdeserialize(void * obj, jmeta_class_t metaClass, cJSON * j, jmode mode) {
	for (int i = 0; i < metaClass.metaSize; i++) {
		const jmeta_t meta = metaClass.meta[i];
		if (!cJSON_HasObjectItem(j, meta.key) && (JMETA_STRICT == mode)) {
			g_error = "key not found";
			return JMETA_ERROR;
		}
		cJSON * jprop = cJSON_GetObjectItem(j, meta.key);
		void * ptr = JMETA_OFFSET(obj, meta.offset);
		if (JOBJECT == meta.type) {
			if (meta.isPointer) {
				// @todo
				void * pointed = getPointedAddress(ptr);
				if (NULL == pointed) {
					// jitem = cJSON_CreateNull();
				} else {
					_jdeserialize(pointed, *(meta.jmetaClass), jprop, mode);
				}
			} else {
				_jdeserialize(ptr, *(meta.jmetaClass), jprop, mode);
			}
		} else if (JARRAY == meta.type) {
			// @todo implement
			if (meta.isPointer) {
				void * pointed = getPointedAddress(ptr);
				if (NULL == pointed) {
					// jitem = cJSON_CreateNull();
				} else {
					// _jdeserialize(pointed, *(meta.jmetaClass), jprop);
				}
			} else {
				// @todo check json array type
				int arrSize = cJSON_GetArraySize(jprop);
				int maxSize = meta.jArrMaxSize;
				printf("%i == %i\n", arrSize, maxSize);
				// @todo check array bounds
				jmeta_class_t itemMetaClass = *meta.jmetaClass;
				// @todo itemMetaClass.isPointer
				for (int j = 0; j < maxSize && j < arrSize; j++) {
					cJSON * jItem = cJSON_GetArrayItem(jprop, j);
					void * elemptr = JMETA_OFFSET(ptr, j * itemMetaClass.size);
					_jdeserialize(elemptr, itemMetaClass, jItem, mode);
				}
			}
		} else if (JSTRING == meta.type) {
			if (meta.isPointer) {
				char ** sptr = (char **)ptr;
				*sptr = malloc(strlen(jprop->valuestring) + 1);
				strcpy(*sptr, jprop->valuestring);
			} else {
				if (strlen(jprop->valuestring) >= meta.jArrMaxSize) {
					g_error = "json string is too long";
					return JMETA_ERROR;
				}
				strcpy(ptr, jprop->valuestring);
			}
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
			g_error = "undefined type";
			return JMETA_ERROR;
		}
	}
	return JMETA_OK; // OK
}

int jdeserializeMode(void * obj, jmeta_class_t metaClass, const char * jstr, jmode mode) {
	//jmeta_class_print(metaClass);  // @test REMOVE
	cJSON * json = cJSON_Parse(jstr);
	int rc = _jdeserialize(obj, metaClass, json, mode);
	cJSON_Delete(json);
	return rc;
}

int jdeserialize(void * obj, jmeta_class_t metaClass, const char * jstr) {
	return jdeserializeMode(obj, metaClass, jstr, JMETA_IGNORE);
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

const char * jGetError() {
	return g_error;
}