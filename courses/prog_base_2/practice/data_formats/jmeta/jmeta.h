#pragma once
#include <stdlib.h>
#include <stddef.h>  /* size_t offsetof(type, member); */

typedef enum jtype {
	JSTRING,
	JINTEGER,
	JDOUBLE,
	JBOOLEAN,
	JARRAY,
	JOBJECT
} jtype;

typedef struct jmeta_s jmeta_t;
typedef struct jmeta_class_s jmeta_class_t;

struct jmeta_s {
	const char * key;
	const jtype type;
	const size_t offset;
	const jmeta_class_t * jmetaClass;
	const size_t jArrMaxSize;
};

struct jmeta_class_s {
	const char *	name;
	const jmeta_t * meta;
	const int 		metaSize;
};

#define JMETA_FIELD_T const jmeta_t
	
#define JMETA_SIZE(JMETACLASS) (sizeof(JMETACLASS)/sizeof(JMETACLASS[0]))

#define JMETA_CLASS_FIELDS(JMETACLASSNAME) JMETACLASSNAME ## _fields

#define JMETA_CLASS(CLASSNAME) static JMETA_FIELD_T JMETA_CLASS_FIELDS(CLASSNAME)[] = 

#define JMETA_CLASS_REG(CLASSNAME) 									\
	static const jmeta_class_t CLASSNAME = { 						\
		.name 		= #CLASSNAME,									\
		.meta 		= JMETA_CLASS_FIELDS(CLASSNAME), 				\
		.metaSize 	= JMETA_SIZE(JMETA_CLASS_FIELDS(CLASSNAME)), 	\
	};

#define JMETA(KEY, JTYPE, STRUCT, SFIELD) \
	(jmeta_t){ KEY, JTYPE, offsetof(STRUCT, SFIELD), NULL, 0 }

#define JMETA_OBJ(KEY, JMETACLASS, STRUCT, SFIELD) \
	(jmeta_t){ KEY, JOBJECT, offsetof(STRUCT, SFIELD), & JMETACLASS, 0 }
	
#define JMETA_ARR(KEY, JMETACLASS, STRUCT, SFIELD, MAXARRLEN) \
	(jmeta_t){ KEY, JARRAY, offsetof(STRUCT, SFIELD), & JMETACLASS, MAXARRLEN }
	
#define JMETA_OFFSET(PTR, OFFSET) ((void *)((char *)PTR + OFFSET))

// returns status
int jfill(void * obj, jmeta_class_t metaClass, const char * jstr);
const char * jtype_toString(jtype type);
