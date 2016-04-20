#include <stdlib.h>
#include <stdio.h>

#include "student.h"

void student_print(student_t * st) {
    printf("Name: %s\nSurname: %s\nScore: %f\nN/A count: %i\nSkip count: %i\n\n",
        st->name, st->surname, st->score, st->naCount, st->skipCount);
}

void student_printList(student_t * arr, int len) {
    for (int i = 0; i < len; i++) {
        student_print(&arr[i]);
    }
}
