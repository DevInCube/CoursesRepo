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
	point_t point;
	point_t points[5];
} test_t;

JMETA_CLASS(JPOINT_T) {
	JMETA("x", JINTEGER, point_t, x),
	JMETA("y", JINTEGER, point_t, y),
};
JMETA_CLASS_REG(JPOINT_T);

JMETA_CLASS(JTEST_T) {
	JMETA("text", 		JSTRING, 	test_t, text),
	JMETA("number", 	JINTEGER, 	test_t, number),
	JMETA("dbl", 		JDOUBLE, 	test_t, dbl),
	JMETA_OBJ("point", 	JPOINT_T, 	test_t, point),
	JMETA_ARR("points", JPOINT_T, 	test_t, points, 5),
};
JMETA_CLASS_REG(JTEST_T);

int main(void) {
	test_t test;
	const char * jsonStr = "{ \"text\": \"TEST\", \"number\": -45, \"dbl\": 13.67, \"point\": { \"x\": 34, \"y\": -4 }, \"points\": [] }";
	int rc = jfill(&test, JTEST_T, jsonStr);
	printf("test struct:\ntext: %s\nnumber: %i\ndbl: %lf\npoint: [x: %i, y: %i]\n", test.text, test.number, test.dbl, test.point.x, test.point.y);
	// if (JMETA_ERROR == rc) {
	// }
	return 0;
}
