#include "args.h"
#include "my_errors.h"

// Парсин опций
int ParseArgs(int argc, char *argv[], struct Options *opts) {
    int flag_err = SUCCESS;
    opts->files = calloc(argc, sizeof(char *));
    if (!opts->files) {
        flag_err = MEMMORY_ERROR;
        fprintf(stderr, "Can't allocate memory.\n");
    }

    for (int i = 1; flag_err == SUCCESS && i < argc; i++) {
        if (argv[i][0] == '-' && argv[i][1] != '\0') {  // разбор опций
            flag_err = ParseFlags(argv[i], opts);
        } else {  // разбор имен файлов
            opts->files[opts->file_count] = argv[i];
            opts->file_count++;
        }
    }

    if (opts->file_count == 0 && flag_err == SUCCESS) {  // если файлов нет
        opts->files[0] = "-";
        opts->file_count++;
    }

    return flag_err;
}

// Разбор одной группы флагов
int ParseFlags(const char *arg, struct Options *opts) {  // написать тесты когда сделаешь генерацию ошибки
    int flag_err = SUCCESS;

    if (strcmp(arg, "--help") == 0) {  // длинный флаг проверяется целиком, один раз
        opts->help = true;
    } else {
        for (int i = 1; arg[i] != '\0' && flag_err == SUCCESS; i++) {  // склейка вида -nE
            if (arg[i] == 'n')
                opts->number_lines = true;
            else if (arg[i] == 'b')
                opts->number_nonblank = true;
            else if (arg[i] == 's')
                opts->squeeze_blank = true;
            else if (arg[i] == 'E')
                opts->show_ends = true;
            else if (arg[i] == 'T')
                opts->show_tabs = true;
            else
                flag_err = UNKNOWN_FLAG_ERROR;  // Написать вывод на экран генерацию ошибки как в cat
        }
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