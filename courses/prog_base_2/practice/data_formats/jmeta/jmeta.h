#pragma once
#include <stdlib.h>
#include <stddef.h>  /* size_t offsetof(type, member); */

typedef enum jerror {
	JMETA_OK	= 0,
	JMETA_ERROR = 1
} jerror;

typedef enum jtype {
	JSTRING,
	JINTEGER,
	JDOUBLE,
	JBOOLEAN,
	JARRAY,
	JOBJECT
} jtype;

typedef enum jmode {
	JMETA_IGNORE 	= 0,
	JMETA_STRICT 	= 1
} jmode;

typedef struct jmeta_s jmeta_t;
typedef struct jmeta_class_s jmeta_class_t;

typedef struct jmeta_field_s {
	const char * key;
	const size_t offset;
} jmeta_field_t;

struct jmeta_s {
	const char * key;
	const jtype type;
	const size_t offset;
	const jmeta_class_t * jmetaClass;
	const size_t jArrMaxSize;
	const int isPointer;
	const size_t arrLenFieldOffset;
};

struct jmeta_class_s {
	const char *	name;
	const size_t	size;
	const jmeta_t * meta;
	const size_t	metaSize;
};

#define __LEN(X) (sizeof(X)/sizeof(X[0]))
#define __JMETA_FIELDS(JMETACLASSNAME) __ ## JMETACLASSNAME ## __fields

#define JMETA_CLASS(CLASSNAME, TYPE, ...)						\
	static const jmeta_t __JMETA_FIELDS(CLASSNAME)[] =	__VA_ARGS__;	\
	static const jmeta_class_t CLASSNAME = { 							\
		.name 		= #CLASSNAME,								\
		.size 		= sizeof(TYPE),								\
		.meta 		= __JMETA_FIELDS(CLASSNAME), 				\
		.metaSize 	= __LEN(__JMETA_FIELDS(CLASSNAME)), 		\
	}

#define JMETA(STRUCT, SFIELD, JTYPE, KEY) \
	/*(jmeta_t)*/{ KEY, JTYPE, offsetof(STRUCT, SFIELD), NULL, 0, 0, 0 }

#define JMETA_AUTO(STRUCT, SFIELD, JTYPE) \
	JMETA(STRUCT, SFIELD, JTYPE, #SFIELD)

#define JMETA_STR(TYPE, FIELDNAME, MAXLENGTH, KEY) \
	/*(jmeta_t)*/{ KEY, JSTRING, offsetof(TYPE, FIELDNAME), NULL, MAXLENGTH, 0, 0 }
	
#define JMETA_STR_AUTO(TYPE, FIELDNAME, MAXLENGTH) \
	JMETA_STR(TYPE, FIELDNAME, MAXLENGTH, #FIELDNAME)
	
#define JMETA_STR_PTR(TYPE, FIELDNAME, KEY) \
	/*(jmeta_t)*/{ KEY, JSTRING, offsetof(TYPE, FIELDNAME), NULL, 0, 1, 0 }
	
#define JMETA_STR_PTR_AUTO(TYPE, FIELDNAME) \
	JMETA_STR_PTR(TYPE, FIELDNAME, #FIELDNAME)

#define JMETA_OBJ(STRUCT, SFIELD, JMETACLASS, KEY) \
	/*(jmeta_t)*/{ KEY, JOBJECT, offsetof(STRUCT, SFIELD), & JMETACLASS, 0, 0, 0 }
	
#define JMETA_OBJ_AUTO(STRUCT, SFIELD, JMETACLASS)	\
	JMETA_OBJ(STRUCT, SFIELD, JMETACLASS, #SFIELD)
	
#define JMETA_OBJ_PTR(STRUCT, SFIELD, JMETACLASS, KEY) \
	/*(jmeta_t)*/{ KEY, JOBJECT, offsetof(STRUCT, SFIELD), & JMETACLASS, 0, 1, 0 }
	
#define JMETA_OBJ_PTR_AUTO(STRUCT, SFIELD, JMETACLASS) \
	JMETA_OBJ_PTR(STRUCT, SFIELD, JMETACLASS, #SFIELD) 
	
#define JMETA_ARR(STRUCT, SFIELD, JMETACLASS, MAXARRLEN, KEY) \
	/*(jmeta_t)*/{ KEY, JARRAY, offsetof(STRUCT, SFIELD), & JMETACLASS, MAXARRLEN, 0, 0 }
	
#define JMETA_ARR_AUTO(STRUCT, SFIELD, JMETACLASS, MAXARRLEN) \
	JMETA_ARR(STRUCT, SFIELD, JMETACLASS, MAXARRLEN, #SFIELD)

#define JMETA_ARR_PTR(STRUCT, SFIELD, JMETACLASS, SLENFIELD, KEY) \
	/*(jmeta_t)*/{ KEY, JARRAY, offsetof(STRUCT, SFIELD), & JMETACLASS, 0, 1, offsetof(STRUCT, SLENFIELD) }
	
#define JMETA_ARR_PTR_AUTO(STRUCT, SFIELD, JMETACLASS, SLENFIELD) \
	JMETA_ARR_PTR(STRUCT, SFIELD, JMETACLASS, SLENFIELD, #SFIELD)
	
#define JMETA_OFFSET(PTR, OFFSET) ((void *)((char *)PTR + OFFSET))

// @todo JMETA statuses
// returns status
int jdeserialize(void * obj, jmeta_class_t metaClass, const char * jstr);
int jdeserializeMode(void * obj, jmeta_class_t metaClass, const char * jstr, jmode mode);
// free string
const char * jserialize(void * obj, jmeta_class_t metaClass);
const char * jGetError();
void jmeta_class_print(jmeta_class_t metaClass);
const char * jtype_toString(jtype type);