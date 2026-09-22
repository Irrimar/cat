#include "cat.h"

#include <stdio.h>

void FileProcessing(FILE *in, FILE *out) {
    int ch;
    while ((ch = fgetc(in)) != EOF) {
        fputc(ch, out);
    }
}