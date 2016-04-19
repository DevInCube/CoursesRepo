#ifndef STUDENT_H_INCLUDED
#define STUDENT_H_INCLUDED

typedef struct student_s {
    int id;
    char name[256];
    char surname[256];
    float score;
    int naCount;
    int skipCount;
} student_t;

void student_print(student_t * st);
void student_printList(student_t * arr, int len);

#endif // STUDENT_H_INCLUDED
