#include <stdio.h>
#include <stdlib.h>

#include "db_manager.h"
#include "student.h"

int main(void) {
    const char * dbFile = "students.db";
    student_t studentList[100];
    db_t * db = db_new(dbFile);

    int countAllStudents = db_countStudents(db);
    printf("Students total: %i\n\n", countAllStudents);
    int filterNACount = 2;
    int filterSkipCount = 7;
    int count = db_getStudentsTask(db, filterNACount, filterSkipCount, studentList, 100);
    printf("Students with N/A > %i or Skips > %i: %i\n\n", filterNACount, filterSkipCount, count);
    student_printList(studentList, count);

    db_free(db);
    return 0;
}
