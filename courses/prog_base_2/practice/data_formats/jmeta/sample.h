#include <stdlib.h>
#include <stdio.h>

#include "jmeta.h"

typedef struct {
	int boolean;
	int integer;
	double _double;
	char string[100];
} basic_t;

JMETA_CLASS(JBASIC_T, basic_t, {
	JMETA_AUTO(basic_t, boolean, JBOOLEAN),
	JMETA_AUTO(basic_t, integer, JINTEGER),
	JMETA_AUTO(basic_t, _double, JDOUBLE),
	JMETA_STR_AUTO(basic_t, string, 100),
});

typedef struct {
	basic_t nested;
} wrapper_t;

JMETA_CLASS(JWRAPPER_T, wrapper_t, {
	JMETA_OBJ_AUTO(wrapper_t, nested, JBASIC_T),
});