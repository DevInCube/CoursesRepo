#ifndef MORSE_H_INCLUDED
#define MORSE_H_INCLUDED

#define VAR 0

char * morse_encode(char *signal, const char *message, int unit_len, int pad_len);
char * morse_decode(char *message, const char *signal);

#endif // MORSE_H_INCLUDED
