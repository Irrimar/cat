// fstat, fileno — POSIX, под -std=c11 без этого макроса не объявлены
#define _POSIX_C_SOURCE 200809L

#include "args.h"

#include <errno.h>
#include <sys/stat.h>

#include "my_errors.h"

// Вторая строка сообщения об ошибке, как у cat
#define HELP_HINT "Try 'cat --help' for more information.\n"

// Справка по использованию. Перечислены только реализованные флаги.
int PrintHelp(FILE *out) {
    int flag_err = SUCCESS;

    fputs(
        "Usage: cat [OPTION]... [FILE]...\n"
        "Concatenate FILE(s) to standard output.\n"
        "\n"
        "With no FILE, or when FILE is -, read standard input.\n"
        "\n"
        "  -b        number nonempty output lines, overrides -n\n"
        "  -E        display $ at end of each line\n"
        "  -n        number all output lines\n"
        "  -s        suppress repeated empty output lines\n"
        "  -T        display TAB characters as ^I\n"
        "      --help  display this help and exit\n"
        "\n"
        "Examples:\n"
        "  cat f - g  Output f's contents, then standard input, then g's contents.\n"
        "  cat        Copy standard input to standard output.\n",
        out);

    // stdout буферизован, поэтому сбой записи проявляется только при сбросе буфера
    if (fflush(out) == EOF) {
        flag_err = OUTPUT_ERROR;
        fprintf(stderr, "cat: write error: %s\n", strerror(errno));
    }

    return flag_err;
}

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
        flag_err = HELP_REQUESTED;  // остальные аргументы уже не читаем
    } else if (arg[1] == '-') {  // остальные длинные флаги не поддерживаются
        flag_err = UNKNOWN_FLAG_ERROR;
        fprintf(stderr, "cat: unrecognized option '%s'\n" HELP_HINT, arg);
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
            else {
                flag_err = UNKNOWN_FLAG_ERROR;
                fprintf(stderr, "cat: invalid option -- '%c'\n" HELP_HINT, arg[i]);
            }
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

    // fopen на каталоге успешно возвращает поток, и только первое чтение падает
    // с EISDIR. Проверяем сразу, иначе каталог выглядел бы как пустой файл.
    struct stat st;
    if (in != NULL && fstat(fileno(in), &st) == 0 && S_ISDIR(st.st_mode)) {
        if (in != stdin) fclose(in);
        errno = EISDIR;
        in = NULL;
    }

    return in;
}

// Закрытие файла и сбос ошибок EOF в stdin
void CloseInput(FILE *in) {
    if (in != stdin)
        fclose(in);
    else
        clearerr(in);
}