#ifndef DB_MANAGER_H_INCLUDED
#define DB_MANAGER_H_INCLUDED

#include "student.h"

typedef struct db_s db_t;

db_t * db_new(const char * dbFile);
void db_free(db_t * self);

int db_insertStudent(db_t * self, student_t * student);
student_t * db_getStudentById(db_t * self, int id);
void db_updateStudent(db_t * self, student_t * student);
void db_deleteStudent(db_t * self, int id);

int db_countStudents(db_t * self);

int db_getStudentsTask(db_t * self, int K, int P, student_t * arr, int arrMax);

#endif // DB_MANAGER_H_INCLUDED
