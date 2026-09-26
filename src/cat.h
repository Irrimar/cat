#ifndef CAT_H
#define CAT_H

#include <stdio.h>

#include "args.h"

struct CatState {
    int line_number;     // сквозной счётчик строк для -n и -b
    bool at_line_start;  // следующий символ начинает новую строку
    bool prev_blank;     // предыдущая строка была пустой — для -s
};

/* Копирует содержимое file_to_read в out, применяя флаги из opts.
   state ведётся между вызовами: нумерация и позиция в строке сквозные
   для всех файлов, поэтому создавать его надо один раз до обхода.
   Возвращает SUCCESS либо OUTPUT_ERROR, если запись в out не удалась;
   сообщение об ошибке печатает вызывающий (errno остаётся от сбоя). */
int FileProcessing(FILE *file_to_read, FILE *out, const struct Options *opts, struct CatState *state);

#endif