#include <stdio.h>

#include "cat.h"

void PrintFile(FILE *file_to_read) {
    int ch;
        while((ch = fgetc(file_to_read))!= EOF) {
            putchar(ch);
        }
}