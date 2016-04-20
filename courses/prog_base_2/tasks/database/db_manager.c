#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <sqlite3.h>

#include "db_manager.h"
#include "student.h"

struct db_s {
    sqlite3 * db;
};

db_t * db_new(const char * dbFile) {
    db_t * self = malloc(sizeof(struct db_s));
    int rc = sqlite3_open(dbFile, & self->db);
    if (SQLITE_ERROR == rc) {
        printf("can't open database\n");
        exit(1);
    }
    return self;
}

void db_free(db_t * self) {
    sqlite3_close(self->db);
    free(self);
}

int db_insertStudent(db_t * self, student_t * student) {
    int newStudentId = -1;
    // @todo
    return newStudentId;
}

student_t * db_getStudentById(db_t * self, int id) {
    // @todo
    return NULL;
}

void db_updateStudent(db_t * self, student_t * student) {
    // @todo
}

void db_deleteStudent(db_t * self, int id) {
    // @todo
}

int db_countStudents(db_t * self) {
    sqlite3_stmt * stmt = NULL;
    sqlite3_prepare_v2(self->db, "SELECT COUNT(*) FROM Student;", -1, &stmt, 0);
    int rc = sqlite3_step(stmt);
    if (SQLITE_ERROR == rc) {
        printf("can't select count\n");
        exit(1);
    }
    int count = sqlite3_column_int(stmt, 0);
    sqlite3_finalize(stmt);
    return count;
}

static void _fillStudent(sqlite3_stmt * stmt, student_t * st) {
    int id = sqlite3_column_int(stmt, 0);
    const unsigned char * name = sqlite3_column_text(stmt, 1);
    const unsigned char * surname = sqlite3_column_text(stmt, 2);
    double score = sqlite3_column_double(stmt, 3);
    int naCount = sqlite3_column_int(stmt, 4);
    int skipCount = sqlite3_column_int(stmt, 5);
    st->id = id;
    strcpy(st->name, (const char *)name);
    strcpy(st->surname, (const char *)surname);
    st->score = score;
    st->naCount = naCount;
    st->skipCount = skipCount;
}

int db_getStudentsTask(db_t * self, int K, int P, student_t * arr, int arrMax) {
    sqlite3_stmt * stmt = NULL;
    const char * sqlQuery = "SELECT * FROM Student WHERE naCount > ? OR skipCount > ?;";
    sqlite3_prepare_v2(self->db, sqlQuery, strlen(sqlQuery), &stmt, 0);
    sqlite3_bind_int(stmt, 1, K);
    sqlite3_bind_int(stmt, 2, P);
    int count = 0;
    while (1) {
        int rc = sqlite3_step(stmt);
        if (SQLITE_ERROR == rc) {
            printf("can't select students\n");
            exit(1);
        } else if (SQLITE_DONE == rc) {
            break;
        } else {
            _fillStudent(stmt, &arr[count]);
            count++;
        }
    }
    sqlite3_finalize(stmt);
    return count;
}
