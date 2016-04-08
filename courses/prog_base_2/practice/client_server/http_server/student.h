#ifndef STUDENT_H_INCLUDED
#define STUDENT_H_INCLUDED

typedef struct {
	char * name;
	char * faculty;
} group_t;

typedef struct {
	char * 	name;
	char * 	surname;
	int 	year;
	float	score;
	group_t group;
} student_t;

student_t
student_empty(void);

const char *
student_toJSON(const student_t * self);

const char *
student_listToJSON(const student_t * const studentList, int studentListSize);

#endif // STUDENT_H_INCLUDED
