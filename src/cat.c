#include "cat.h"

#include <stdio.h>

#include "args.h"
#include "my_errors.h"

// Нужен ли номер перед этим символом. Только отвечает на вопрос.
static bool NeedLineNumber(const struct Options *opts, int ch) {
    bool need = opts->number_lines;  // -n: все строки

    if (opts->number_nonblank) need = (ch != '\n');  // -b перекрывает -n

    return need;
}

// -n, -b: печатает номер в начале строки и сдвигает счётчик
static void PrintLineNumber(FILE *out, const struct Options *opts, struct CatState *state, int ch) {
    if (state->at_line_start && NeedLineNumber(opts, ch)) {
        fprintf(out, "%6d\t", state->line_number);  // табуляция своя, -T её не касается
        state->line_number++;
    }
}

// -s: true, если эта пустая строка идёт сразу за пустой и выводить её не надо
static bool SkipBlankLine(const struct Options *opts, struct CatState *state, int ch) {
    bool skip = false;

    if (state->at_line_start) {  // решение принимается по первому символу строки
        bool blank = (ch == '\n');

        if (blank && state->prev_blank && opts->squeeze_blank) skip = true;

        state->prev_blank = blank;
    }

    return skip;
}

// -E, -T: как вывести сам входной символ
static void PrintChar(FILE *out, const struct Options *opts, int ch) {
    if (ch == '\n' && opts->show_ends) fputc('$', out);

    if (ch == '\t' && opts->show_tabs)
        fputs("^I", out);
    else
        fputc(ch, out);
}

int FileProcessing(FILE *in, FILE *out, const struct Options *opts, struct CatState *state) {
    int flag_err = SUCCESS;
    int ch;

    while ((ch = fgetc(in)) != EOF) {
        if (!SkipBlankLine(opts, state, ch)) {
            PrintLineNumber(out, opts, state, ch);  // -n, -b: префикс перед символом
            PrintChar(out, opts, ch);               // -E, -T: как вывести сам символ
        }

        state->at_line_start = (ch == '\n');  // позиция в строке не зависит от флагов
    }

    // Вывод буферизован: сбой записи проявляется только при сбросе буфера.
    // Одна проверка на весь файл вместо проверки каждого fputc.
    if (fflush(out) == EOF || ferror(out)) flag_err = OUTPUT_ERROR;

    return flag_err;
}
