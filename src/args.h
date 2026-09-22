#ifndef ARGS_H
#define ARGS_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Options {
    bool number_lines;     // -n
    bool number_nonblank;  // -b
    bool squeeze_blank;    // -s
    bool show_ends;        // -E
    bool show_tabs;        // -T
    bool help;             // --help

    int file_count;
    char **files;
};

int ParseArgs(int argc, char *argv[], struct Options *opts);
void OptionsFree(struct Options *opts);

FILE *OpenInput(const char *path_to_file);
void CloseInput(FILE *in);

#endif