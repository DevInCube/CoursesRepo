#include <stdlib.h>
#include <stdio.h>

#include "jmeta.h"
#include "sample.h"

typedef struct {
	int x;
	int y;
} point_t;

typedef struct {
	char * text;
	char buf[20];
	int number;
	double dbl;
	int boolean;
	point_t point;
	point_t * pPoint;
	point_t * pNullPoint;
	point_t points[5];
	point_t * pPoints;
	int pPointLen;
} test_t;

JMETA_CLASS(JPOINT_T, point_t, {
	JMETA_AUTO(point_t, x, JINTEGER),
	JMETA_AUTO(point_t, y, JINTEGER),
});

JMETA_CLASS(JTEST_T, test_t, {
	JMETA_AUTO(test_t, number, JINTEGER),
	JMETA_AUTO(test_t, dbl, JDOUBLE),
	JMETA_AUTO(test_t, boolean, JBOOLEAN),
	JMETA_STR_PTR_AUTO(test_t, text),
	JMETA_STR_AUTO(test_t, buf, 20),
	JMETA_OBJ_PTR_AUTO(test_t, pPoint, JPOINT_T),
	JMETA_OBJ_PTR_AUTO(test_t, pNullPoint, JPOINT_T),
	JMETA_OBJ_AUTO(test_t, point, JPOINT_T),
	JMETA_ARR_AUTO(test_t, points, JPOINT_T, 5),
	JMETA_ARR_PTR_AUTO(test_t, pPoints, JPOINT_T, pPointLen),
});

int main(void) {
	basic_t basic = {
		.boolean = 1,
		.integer = 3,
		._double = 11.465,
		.string  = "The string."
	};
	const char * basicStr = jserialize(&basic, JBASIC_T);
	printf("%s\n", basicStr);
	free((void *)basicStr);
	
	wrapper_t wrapper = { 
		.nested = basic
	};
	const char * wrapperStr = jserialize(&wrapper, JWRAPPER_T);
	printf("%s\n", wrapperStr);
	free((void *)wrapperStr);
	
	basic_t basic_empty;
	basicStr = "{"
		"\"boolean\": false,"
		"\"integer\": 23,"
		"\"_double\": 31.13,"
		"\"string\": \"Something\","
	"}";
	printf("%s\n", basicStr);
	int rc = jdeserialize(&basic_empty, JBASIC_T, basicStr);
	if (JMETA_ERROR == rc) {
		printf("Error: %s\n", jGetError());
		return 1;
	}
	basicStr = jserialize(&basic_empty, JBASIC_T);
	printf("%s\n", basicStr);
	free((void *)basicStr);
	
	return 0;/*
	//jmeta_class_print(JTEST_T);
	point_t innerPoint = {6, 6};
	point_t pp[] = {{5, 5}, {3, 3}};
	test_t test = {
		.text = "Default",
		.buf = "Str",
		.number = 13,
		.dbl = 66.6,
		.boolean = 0,
		.pPoint = &innerPoint,
		.point = {
			.x = 7,
			.y = -7
		},
		.points = {
			{1, -1},
			{-2, 2},
		},
		.pPoints = pp,
		.pPointLen = 2
	};
	const char * jstr = jserialize(&test, JTEST_T);
	printf("%s\n", jstr);
	free((void *)jstr);
	const char * jsonStr = 
		"{ \"text\": \"TEST\", \"buf\": \"OVERFLOW\", \"number\": -45, \"dbl\": 13.67, \"boolean\": true, "
		" \"pPoint\" : { \"x\": -34, \"y\": 5 }, \"point\": { \"x\": 34, \"y\": -4 },"
		" \"points\": [{ \"x\": -14, \"y\": 15 }, { \"x\": 24, \"y\": 25 }, { \"x\": 34, \"y\": 35 }] }";
	int rc = jdeserialize(&test, JTEST_T, jsonStr);
	if (JMETA_ERROR == rc) {
		printf("Error at deserialization. %s\n", jGetError());
		return 1;
	}
	printf("test struct:\ntext: %s\nnumber: %i\ndbl: %lf\nboolean: %i\npoint: [x: %i, y: %i]\n", 
		test.text, test.number, test.dbl, test.boolean, test.point.x, test.point.y);
	if (test.pPoint != NULL) {
		printf("pPoint: [%i, %i]\n", test.pPoint->x, test.pPoint->y);
	}
	for (int i = 0; i < 5; i++) {
		printf("points[%i]: [%i, %i]\n", i, test.points[i].x, test.points[i].y);
	}
	return 0;*/
}