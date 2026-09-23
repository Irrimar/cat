#include <errno.h>  // Используются для получения ошибок как в cat
#include <stdbool.h>

#include "args.h"
#include "cat.h"

int main(int argc, char *argv[]) {
    struct Options opts = {0};
    bool flag_err = ParseArgs(argc, argv, &opts);

    // обработать ошибку

    for (int i = 0; i < opts.file_count; i++) {
        const char *path_to_file = opts.files[i];

        FILE *in = OpenInput(path_to_file);
        if (in == NULL) {
            fprintf(stderr, "cat: %s: %s\n", path_to_file, strerror(errno));
            flag_err = true;
        } else {
            FileProcessing(in, stdout);
            CloseInput(in);
        }
    }

    OptionsFree(&opts);
    return flag_err;
}