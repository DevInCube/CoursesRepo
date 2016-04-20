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
	point_t * pPoint;
	point_t * pNullPoint;
	point_t points[5];
	point_t * pPoints;
	int pPointLen;
} test_t;

JMETA_CLASS(JPOINT_T) {
	JMETA_AUTO(point_t, x, JINTEGER),
	JMETA_AUTO(point_t, y, JINTEGER),
};
JMETA_CLASS_REG(JPOINT_T, point_t);

JMETA_CLASS(JTEST_T) {
	JMETA(test_t, text, JSTRING, "text"),
	JMETA(test_t, number, JINTEGER, "number"),
	JMETA(test_t, dbl, JDOUBLE, "dbl"),
	JMETA(test_t, boolean, JBOOLEAN, "boolean"),
	JMETA_OBJ_PTR(test_t, pPoint, JPOINT_T, "pPoint"),
	JMETA_OBJ_PTR(test_t, pNullPoint, JPOINT_T, "pNullPoint"),
	JMETA_OBJ(test_t, point, JPOINT_T, "point"),
	JMETA_ARR(test_t, points, JPOINT_T, 5, "points"),
	JMETA_ARR_PTR(test_t, pPoints, JPOINT_T, pPointLen, "pPoints"),
};
JMETA_CLASS_REG(JTEST_T, test_t);

int main(void) {
	point_t innerPoint = {6, 6};
	point_t pp[] = {{5, 5}, {3, 3}};
	test_t test = {
		.text = "Default",
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
		"{ \"text\": \"TEST\", \"number\": -45, \"dbl\": 13.67, \"boolean\": true, "
		" \"pPoint\" : { \"x\": -34, \"y\": 5 }, \"point\": { \"x\": 34, \"y\": -4 },"
		" \"points\": [{ \"x\": -14, \"y\": 15 }, { \"x\": 24, \"y\": 25 }, { \"x\": 34, \"y\": 35 }] }";
	int rc = jdeserialize(&test, JTEST_T, jsonStr);
	printf("test struct:\ntext: %s\nnumber: %i\ndbl: %lf\nboolean: %i\npoint: [x: %i, y: %i]\n", 
		test.text, test.number, test.dbl, test.boolean, test.point.x, test.point.y);
	if (test.pPoint != NULL) {
		printf("pPoint: [%i, %i]\n", test.pPoint->x, test.pPoint->y);
	}
	for (int i = 0; i < 5; i++) {
		printf("points[%i]: [%i, %i]\n", i, test.points[i].x, test.points[i].y);
	}
	// if (JMETA_ERROR == rc) {
	// }
	return 0;
}