#include <stdlib.h>
#include <stdio.h>
#include "jmeta.h"

typedef struct {
	int x;
	int y;
} point_t;

typedef struct {
	char * text;
	int number;
	double dbl;
	int boolean;
	point_t point;
	point_t points[5];
} test_t;

JMETA_CLASS(JPOINT_T) {
	JMETA("x", JINTEGER, point_t, x),
	JMETA("y", JINTEGER, point_t, y),
};
JMETA_CLASS_REG(JPOINT_T, point_t);

JMETA_CLASS(JTEST_T) {
	JMETA("text", 		JSTRING, 	test_t, text),
	JMETA("number", 	JINTEGER, 	test_t, number),
	JMETA("dbl", 		JDOUBLE, 	test_t, dbl),
	JMETA("boolean", 	JBOOLEAN, 	test_t, boolean),
	JMETA_OBJ("point", 	JPOINT_T, 	test_t, point),
	JMETA_ARR("points", JPOINT_T, 	test_t, points, 5),
};
JMETA_CLASS_REG(JTEST_T, test_t);

int main(void) {
	test_t test = {
		.text = "Default",
		.number = 13,
		.dbl = 66.6,
		.boolean = 0,
		.point = {
			.x = 7,
			.y = -7
		},
		.points = {
			{1, -1},
			{-2, 2},
		}
	};
	const char * jstr = jserialize(&test, JTEST_T);
	printf("%s\n", jstr);
	free((void *)jstr);
	const char * jsonStr = 
		"{ \"text\": \"TEST\", \"number\": -45, \"dbl\": 13.67, \"boolean\": true, "
		" \"point\": { \"x\": 34, \"y\": -4 }, \"points\": [] }";
	int rc = jfill(&test, JTEST_T, jsonStr);
	printf("test struct:\ntext: %s\nnumber: %i\ndbl: %lf\nboolean: %i\npoint: [x: %i, y: %i]\n", 
		test.text, test.number, test.dbl, test.boolean, test.point.x, test.point.y);
	// if (JMETA_ERROR == rc) {
	// }
	return 0;
}
