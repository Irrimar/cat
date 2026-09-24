#include <errno.h>  // Используются для получения ошибок как в cat
#include <stdbool.h>

#include "args.h"
#include "cat.h"
#include "my_errors.h"

int main(int argc, char *argv[]) {
    struct Options opts = {0};
    int flag_err = ParseArgs(argc, argv, &opts);

    if (flag_err == SUCCESS) {
        for (int i = 0; i < opts.file_count; i++) {
            const char *path_to_file = opts.files[i];

            FILE *in = OpenInput(path_to_file);
            if (in == NULL) {
                fprintf(stderr, "cat: %s: %s\n", path_to_file, strerror(errno));
                flag_err = INPUT_ERROR;
            } else {
                FileProcessing(in, stdout);
                CloseInput(in);
            }
        }
    }

    OptionsFree(&opts);
    return flag_err != SUCCESS ? 1 : 0;  // наружу только 0 или 1, как у cat
}