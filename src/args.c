#include "args.h"

#include <stdio.h>

struct Options parse_args(int argc, char *argv[]) {
    struct Options opts = {0};

    return opts;
}

// for (int  i = 0; i < argc; i++) {
//     printf("argc = %d\nargv[%d] = %s\n", argc, i, argv[i]);
// }

// parse_args

/*for (int i = 1; i < argc; i++) {
   if (argv[i][0] == '-' && argv[i][1] != '\0') {
        // флаг: argv[i][1] — буква ('n', 'b', 's'...)
        // или strcmp(argv[i], "--help") == 0
    } else {
        // имя файла (или "-" — stdin)
    }*/

FILE *open_input(const char *path_to_file) {
    FILE *in;

    if (path_to_file[0] == '-')
        in = stdin;
    else
        in = fopen(path_to_file, "r");

    return in;
}

void close_input(FILE *in) {
    if (in != stdin) fclose(in);
}