#ifndef ARGS_H
#define ARGS_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Результат разбора командной строки.
   Создавать только нулевой: struct Options opts = {0};
   Массив files выделяется в ParseArgs и освобождается через OptionsFree.
   Сами строки не копируются — это указатели внутрь argv или на литерал "-",
   освобождать их нельзя. */
struct Options {
    bool number_lines;     // -n
    bool number_nonblank;  // -b
    bool squeeze_blank;    // -s
    bool show_ends;        // -E
    bool show_tabs;        // -T
    bool help;             // --help

    int file_count;  // сколько имён лежит в files
    char **files;  // имена файлов в порядке из командной строки, "-" означает stdin
};

/* Разбирает argv и заполняет opts. Если файлов в аргументах нет,
   подставляет единственный элемент "-", чтобы читать stdin.
   Возвращает 0 либо код ошибки из my_errors.h; при ошибке разбор
   прерывается и содержимое files не определено.
   Выделяет память: после вызова нужен OptionsFree (в том числе при ошибке). */
int ParseArgs(int argc, char *argv[], struct Options *opts);

/* Разбирает одну группу флагов: "-n", склейку вида "-nE" или "--help",
   и взводит соответствующие поля opts.
   Ожидает аргумент, начинающийся с '-' и длиннее одного символа.
   Возвращает SUCCESS либо UNKNOWN_FLAG_ERROR на первом неизвестном символе;
   флаги, разобранные до ошибки, остаются взведёнными. */
int ParseFlags(const char *arg, struct Options *opts);

/* Освобождает массив files, обнуляет указатель и file_count.
   Вызывать повторно безопасно. Парная к ParseArgs. */
void OptionsFree(struct Options *opts);

/* Возвращает stdin для имени "-", иначе открывает файл на чтение.
   При неудаче возвращает NULL, причина — в errno.
   Открытый поток закрывать только через CloseInput. */
FILE *OpenInput(const char *path_to_file);

/* Закрывает поток, полученный от OpenInput.
   stdin не закрывает, а сбрасывает у него признак EOF: поток один на всю
   программу и может понадобиться снова (cat a.txt - b.txt -).
   Парная к OpenInput. */
void CloseInput(FILE *in);

#endif