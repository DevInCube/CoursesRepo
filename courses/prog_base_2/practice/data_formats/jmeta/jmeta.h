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
	const int isPointer;
	const size_t arrLenOffset;
};

struct jmeta_class_s {
	const char *	name;
	const size_t	size;
	const jmeta_t * meta;
	const size_t	metaSize;
};

#define JMETA_FIELD_T const jmeta_t
	
#define JMETA_SIZE(JMETACLASS) (sizeof(JMETACLASS)/sizeof(JMETACLASS[0]))

#define JMETA_CLASS_FIELDS(JMETACLASSNAME) JMETACLASSNAME ## _fields

#define JMETA_CLASS(CLASSNAME) static JMETA_FIELD_T JMETA_CLASS_FIELDS(CLASSNAME)[] = 

#define JMETA_CLASS_REG(CLASSNAME, STRUCT)							\
	static const jmeta_class_t CLASSNAME = { 						\
		.name 		= #CLASSNAME,									\
		.size 		= sizeof(STRUCT),								\
		.meta 		= JMETA_CLASS_FIELDS(CLASSNAME), 				\
		.metaSize 	= JMETA_SIZE(JMETA_CLASS_FIELDS(CLASSNAME)), 	\
	};

#define JMETA(STRUCT, SFIELD, JTYPE, KEY) \
	(jmeta_t){ KEY, JTYPE, offsetof(STRUCT, SFIELD), NULL, 0, 0 }

#define JMETA_AUTO(STRUCT, SFIELD, JTYPE) \
	JMETA(STRUCT, SFIELD, JTYPE, #SFIELD)

#define JMETA_OBJ(STRUCT, SFIELD, JMETACLASS, KEY) \
	(jmeta_t){ KEY, JOBJECT, offsetof(STRUCT, SFIELD), & JMETACLASS, 0, 0 }
	
#define JMETA_OBJ_PTR(STRUCT, SFIELD, JMETACLASS, KEY) \
	(jmeta_t){ KEY, JOBJECT, offsetof(STRUCT, SFIELD), & JMETACLASS, 0, 1 }
	
#define JMETA_ARR(STRUCT, SFIELD, JMETACLASS, MAXARRLEN, KEY) \
	(jmeta_t){ KEY, JARRAY, offsetof(STRUCT, SFIELD), & JMETACLASS, MAXARRLEN, 0 }

#define JMETA_ARR_PTR(STRUCT, SFIELD, JMETACLASS, SLENFIELD, KEY) \
	(jmeta_t){ KEY, JARRAY, offsetof(STRUCT, SFIELD), & JMETACLASS, 0, 1, offsetof(STRUCT, SLENFIELD) }
	
#define JMETA_OFFSET(PTR, OFFSET) ((void *)((char *)PTR + OFFSET))

// @todo JMETA statuses
// returns status
int jdeserialize(void * obj, jmeta_class_t metaClass, const char * jstr);
// free string
const char * jserialize(void * obj, jmeta_class_t metaClass);
const char * jtype_toString(jtype type);