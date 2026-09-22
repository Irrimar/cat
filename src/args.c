#include "args.h"

int ParseArgs(int argc, char *argv[], struct Options *opts) {
    bool flag_err = false;
    opts->files = calloc(argc, sizeof(char *));  // количестов передалать
    if (!opts->files) {
        flag_err = true;
        fprintf(stderr, "Can't allocate memory.\n");
    }

    for (int i = 1; flag_err == false && i < argc; i++) {
        if (argv[i][0] == '-' && argv[i][1] != '\0') {
            // флаг: argv[i][1] — буква ('n', 'b', 's'...)
            // или strcmp(argv[i], "--help") == 0
        } else if (argv[i][0] == '-' && argv[i][1] == '\0') {
            opts->files[i - 1] = "-";
            opts->file_count++;
        } else {
            opts->files[i - 1] = argv[i];
            opts->file_count++;
        }
    }

    if (opts->file_count == 0 && flag_err == false) {
        opts->files[0] = "-";
        opts->file_count++;
    }

    return flag_err;
}

// for (int  i = 0; i < argc; i++) {
//     printf("argc = %d\nargv[%d] = %s\n", argc, i, argv[i]);
// }

void OptionsFree(struct Options *opts) { free(opts->files); }

FILE *OpenInput(const char *path_to_file) {
    FILE *in;

    if (strcmp(path_to_file, "-") == 0)
        in = stdin;
    else
        in = fopen(path_to_file, "r");

    return in;
}

void CloseInput(FILE *in) {
    if (in != stdin)
        fclose(in);
    else
        clearerr(in);
}