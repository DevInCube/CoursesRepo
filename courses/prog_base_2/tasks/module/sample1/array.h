#ifndef ARRAY_H_INCLUDED
#define ARRAY_H_INCLUDED

typedef struct array_s array_t;

array_t * array_new(int size);
void array_free(array_t * self);

int array_getSize(const array_t * self);
int array_getValueAt(const array_t * self, int pos);
void array_setValueAt(array_t * self, int pos, int val);

void array_fill(array_t * self);
void array_print(const array_t * self);

#endif // ARRAY_H_INCLUDED
