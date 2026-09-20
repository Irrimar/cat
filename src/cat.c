#include <stdio.h>

#include "cat.h"

void FileProcessing(FILE *file_to_read, FILE *out) {
    int ch;
        while((ch = fgetc(file_to_read))!= EOF) {
            fputc(ch, out);
        }
}