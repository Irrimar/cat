#ifndef ARGS_H
#define ARGS_H

#include <stdbool.h>


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

struct Options parse_args(int argc, char *argv[]);

FILE *open_input(const char *path_to_file);
void close_input(FILE *in);

#endif