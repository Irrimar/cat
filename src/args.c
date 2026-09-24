#include "args.h"

// Парсин опций
int ParseArgs(int argc, char *argv[], struct Options *opts) {
    int flag_err = 0;
    opts->files = calloc(argc, sizeof(char *));
    if (!opts->files) {
        flag_err = 1;
        fprintf(stderr, "Can't allocate memory.\n");
    }

    for (int i = 1; flag_err == 0 && i < argc; i++) {
        if (argv[i][0] == '-' && argv[i][1] != '\0') {  // разбор опций
            int j = 1;
            do {
                if (argv[i][j] == 'n')
                    opts->number_lines = true;
                else if (argv[i][j] == 'b')
                    opts->number_nonblank = true;
                else if (argv[i][j] == 's')
                    opts->squeeze_blank = true;
                else if (argv[i][j] == 'E')
                    opts->show_ends = true;
                else if (argv[i][j] == 'T')
                    opts->show_tabs = true;
                else if (strcmp(argv[i], "--help") == 0)
                    opts->help = true;

                else {
                    flag_err = 2;  // Написать генерацию ошибки
                }

                j++;
            } while (argv[i][j] != '\0' && flag_err == 0);

        } else if (argv[i][0] == '-' && argv[i][1] == '\0') {  // -
            opts->files[opts->file_count] = "-";
            opts->file_count++;
        } else {  // разбор имен файлов
            opts->files[opts->file_count] = argv[i];
            opts->file_count++;
        }
    }

    if (opts->file_count == 0 && flag_err == 0) {  // если файлов нет
        opts->files[0] = "-";
        opts->file_count++;
    }

    return flag_err;
}

// Освобождение памяти
void OptionsFree(struct Options *opts) {
    free(opts->files);
    opts->files = NULL;
    opts->file_count = 0;
}

// Откытие файла на чтение или передача потока stdin
FILE *OpenInput(const char *path_to_file) {
    FILE *in;

    if (strcmp(path_to_file, "-") == 0)
        in = stdin;
    else
        in = fopen(path_to_file, "r");

    return in;
}

// Закрытие файла и сбос ошибок EOF в stdin
void CloseInput(FILE *in) {
    if (in != stdin)
        fclose(in);
    else
        clearerr(in);
}