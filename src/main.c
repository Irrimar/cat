#include <errno.h>  // как в cat
#include <stdbool.h>
#include <stdio.h>
#include <string.h>  // Используются для получения ошибок

#include "args.h"
#include "cat.h"

int main(int argc, char *argv[]) {
    struct Options opts = parse_args(argc, argv);

    bool flag_err = false;
    for (int i = 1; i < argc; i++) {  // потом переделать
        // const char *path_to_file = argv[i];
        const char *path_to_file = "-";
        FILE *in = open_input(path_to_file);

        if (in == NULL) {
            fprintf(stderr, "cat: %s: %s\n", path_to_file, strerror(errno));
            flag_err = true;
        } else {
            FileProcessing(in, stdout);
            close_input(in);
        }
    }

    return flag_err;
}