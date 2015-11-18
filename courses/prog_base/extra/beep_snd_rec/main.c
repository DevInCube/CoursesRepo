// simple sounds via Beep(frequency_hrz, duration_ms)
// on the PC internal speaker, a Windows Console program
#include <stdio.h>
#include <time.h>

#include "play.h"

struct Note {
    int freq;
    int code;
};

struct TimedNote {
    struct Note * note;
    int pauseTime;
};

int main(void)
{
    struct Note notes[] = {
        { 523, 'q' },
        { 587, 'w' },
        { 659, 'e' },
        { 698, 'r' },
        { 783, 't' },
        { 880, 'y' },
        { 987, 'u' },
        { 554, '2' },
        { 622, '3' },
        { 739, '5' },
        { 830, '6' },
        { 932, '7' },
    };
    int max_notes = 80;
    struct TimedNote record[max_notes + 1];
    record[max_notes].note = NULL;
    clock_t start;
    char c, i;
    for(i = 0; i < max_notes; i++) {
        record[i].note = NULL;
    }
    int cursor = 0;
    int isRecording = 0;
    int size = sizeof(notes) / sizeof(notes[0]);
    puts("Hello");
    while((c = _getch()) != 27) {  /* ESC */
        switch(c) {
            case 's':
                isRecording = !isRecording;
                if (isRecording) {
                    start = clock();
                }
                puts((isRecording) ? "\nStart recording:" : "\nStop recording.");
                cursor = 0;
                break;
            case 'p':
                puts("\nStart play:");
                i = 0;
                while(record[i].note != NULL) {
                    _beep(record[i].note->freq, 250);
                    _sleep(record[i].pauseTime - 250);
                    i++;
                    printf("#");
                }
                puts("\nPlay stopped:");
                break;
            default:
                for(i = 0; i < size; i++) {
                    if (notes[i].code == c) {
                        _beep(notes[i].freq, 250);
                        if(isRecording) {
                            record[cursor].note = &notes[i];
                            record[cursor].pauseTime = (clock() - start) * 1000 / CLOCKS_PER_SEC;  //msec
                            start = clock();
                            cursor++;
                            printf("#");
                            if(cursor == max_notes) {
                                isRecording = 0;
                                puts("\nStop recording.");
                            }
                        }
                    }
                }
        }
    }

    puts("\nPress any key...\n");
    _getch(); // key wait
    return 0;
}
