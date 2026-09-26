#include <errno.h>  // Используются для получения ошибок как в cat
#include <stdbool.h>

#include "args.h"
#include "cat.h"
#include "my_errors.h"

int main(int argc, char *argv[]) {
    struct Options opts = {0};
    int flag_err = ParseArgs(argc, argv, &opts);

    struct CatState state;  // счёт с единицы, первый символ начинает строку
    state.line_number = 1;
    state.at_line_start = true;
    state.prev_blank = false;

    if (flag_err == HELP_REQUESTED) {
        flag_err = PrintHelp(stdout);  // справка сама по себе не ошибка: SUCCESS, если записалась
    } else if (flag_err == SUCCESS) {
        for (int i = 0; i < opts.file_count && flag_err != OUTPUT_ERROR; i++) {
            const char *path_to_file = opts.files[i];

            FILE *in = OpenInput(path_to_file);
            if (in == NULL) {
                fprintf(stderr, "cat: %s: %s\n", path_to_file, strerror(errno));
                flag_err = INPUT_ERROR;
            } else {
                int write_err = FileProcessing(in, stdout, &opts, &state);

                if (write_err != SUCCESS) {  // errno ещё от сбоя записи, до CloseInput
                    fprintf(stderr, "cat: write error: %s\n", strerror(errno));
                    flag_err = write_err;
                }

                CloseInput(in);
            }
        }
    }

    OptionsFree(&opts);
    return flag_err != SUCCESS ? 1 : 0;  // наружу только 0 или 1, как у cat
}