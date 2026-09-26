#define _GNU_SOURCE

#include "../src/cat.h"
#include "minunit.h"

static int test_copies_input(void);
static int test_copies_empty_input(void);
static int test_copies_multiline(void);
static int test_copies_utf8(void);
static int test_number_lines(void);
static int test_number_counts_blank(void);
static int test_number_no_trailing_newline(void);
static int test_number_nonblank_skips_blank(void);
static int test_number_nonblank_overrides(void);
static int test_squeeze_collapses(void);
static int test_squeeze_keeps_single(void);
static int test_squeeze_leading_blanks(void);
static int test_show_ends(void);
static int test_show_ends_no_trailing_newline(void);
static int test_show_tabs(void);
static int test_number_tab_not_converted(void);
static int test_all_flags_together(void);
static int test_number_updates_state(void);
static int test_number_continues_across_calls(void);
static int test_unfinished_line_continues(void);

int main(void) {
    mu_run_test(test_copies_input);
    mu_run_test(test_copies_empty_input);
    mu_run_test(test_copies_multiline);
    mu_run_test(test_copies_utf8);
    mu_run_test(test_number_lines);
    mu_run_test(test_number_counts_blank);
    mu_run_test(test_number_no_trailing_newline);
    mu_run_test(test_number_nonblank_skips_blank);
    mu_run_test(test_number_nonblank_overrides);
    mu_run_test(test_squeeze_collapses);
    mu_run_test(test_squeeze_keeps_single);
    mu_run_test(test_squeeze_leading_blanks);
    mu_run_test(test_show_ends);
    mu_run_test(test_show_ends_no_trailing_newline);
    mu_run_test(test_show_tabs);
    mu_run_test(test_number_tab_not_converted);
    mu_run_test(test_all_flags_together);
    mu_run_test(test_number_updates_state);
    mu_run_test(test_number_continues_across_calls);
    mu_run_test(test_unfinished_line_continues);

    mu_report();
    return mu_tests_failed != 0;
}

// Прогоняет FileProcessing над строкой input, вывод кладёт в out_buf.
// Флаги и состояние задаёт вызывающий — состояние можно проверить после вызова.
// Возвращает 0, если всё получилось, иначе 1 (не удалось открыть потоки).
static int RunFileProcessing(char *input, const struct Options *opts, struct CatState *state, char *out_buf,
                             size_t size) {
    int err = 1;

    out_buf[0] = '\0';

    FILE *in = fmemopen(input, strlen(input), "r");
    FILE *out = tmpfile();

    if (in != NULL && out != NULL) {
        err = FileProcessing(in, out, opts, state);  // SUCCESS, если запись удалась

        rewind(out);
        size_t len = fread(out_buf, 1, size - 1, out);
        out_buf[len] = '\0';
    }

    if (in != NULL) fclose(in);
    if (out != NULL) fclose(out);

    return err;
}

// без флагов: текст копируется байт в байт
static int test_copies_input(void) {
    char input[] = "abc";
    char expected[] = "abc";
    char buf[256];

    struct Options opts = {0};  // без флагов
    struct CatState state = {1, true, false};

    mu_assert_inteq(0, RunFileProcessing(input, &opts, &state, buf, sizeof(buf)));
    mu_assert_streq(expected, buf);

    return 0;
}

// без флагов: пустой вход — пустой выход, цикл не выполняется
static int test_copies_empty_input(void) {
    char input[] = "";
    char expected[] = "";
    char buf[256];

    struct Options opts = {0};  // без флагов
    struct CatState state = {1, true, false};

    mu_assert_inteq(0, RunFileProcessing(input, &opts, &state, buf, sizeof(buf)));
    mu_assert_streq(expected, buf);

    return 0;
}

// без флагов: переводы строк сохраняются
static int test_copies_multiline(void) {
    char input[] = "a\nb\n";
    char expected[] = "a\nb\n";
    char buf[256];

    struct Options opts = {0};  // без флагов
    struct CatState state = {1, true, false};

    mu_assert_inteq(0, RunFileProcessing(input, &opts, &state, buf, sizeof(buf)));
    mu_assert_streq(expected, buf);

    return 0;
}

// без флагов: многобайтовые символы проходят без искажений
static int test_copies_utf8(void) {
    char input[] = "лол\n";
    char expected[] = "лол\n";
    char buf[256];

    struct Options opts = {0};  // без флагов
    struct CatState state = {1, true, false};

    mu_assert_inteq(0, RunFileProcessing(input, &opts, &state, buf, sizeof(buf)));
    mu_assert_streq(expected, buf);

    return 0;
}

// -n: номер перед каждой строкой, шесть позиций и табуляция
static int test_number_lines(void) {
    char input[] = "a\nb\n";
    char expected[] = "     1\ta\n     2\tb\n";
    char buf[256];

    struct Options opts = {0};
    opts.number_lines = true;
    struct CatState state = {1, true, false};

    mu_assert_inteq(0, RunFileProcessing(input, &opts, &state, buf, sizeof(buf)));
    mu_assert_streq(expected, buf);

    return 0;
}

// -n: пустые строки тоже нумеруются
static int test_number_counts_blank(void) {
    char input[] = "a\n\nb\n";
    char expected[] = "     1\ta\n     2\t\n     3\tb\n";
    char buf[256];

    struct Options opts = {0};
    opts.number_lines = true;
    struct CatState state = {1, true, false};

    mu_assert_inteq(0, RunFileProcessing(input, &opts, &state, buf, sizeof(buf)));
    mu_assert_streq(expected, buf);

    return 0;
}

// -n: последняя строка без перевода строки
static int test_number_no_trailing_newline(void) {
    char input[] = "a\nb";
    char expected[] = "     1\ta\n     2\tb";
    char buf[256];

    struct Options opts = {0};
    opts.number_lines = true;
    struct CatState state = {1, true, false};

    mu_assert_inteq(0, RunFileProcessing(input, &opts, &state, buf, sizeof(buf)));
    mu_assert_streq(expected, buf);

    return 0;
}

// -b: пустые строки не нумеруются и не тратят номер
static int test_number_nonblank_skips_blank(void) {
    char input[] = "a\n\nb\n";
    char expected[] = "     1\ta\n\n     2\tb\n";
    char buf[256];

    struct Options opts = {0};
    opts.number_nonblank = true;
    struct CatState state = {1, true, false};

    mu_assert_inteq(0, RunFileProcessing(input, &opts, &state, buf, sizeof(buf)));
    mu_assert_streq(expected, buf);

    return 0;
}

// -b перекрывает -n независимо от того, что оба взведены
static int test_number_nonblank_overrides(void) {
    char input[] = "a\n\nb\n";
    char expected[] = "     1\ta\n\n     2\tb\n";
    char buf[256];

    struct Options opts = {0};
    opts.number_lines = true;
    opts.number_nonblank = true;
    struct CatState state = {1, true, false};

    mu_assert_inteq(0, RunFileProcessing(input, &opts, &state, buf, sizeof(buf)));
    mu_assert_streq(expected, buf);

    return 0;
}

// -s: три пустые строки подряд схлопываются в одну
static int test_squeeze_collapses(void) {
    char input[] = "a\n\n\n\nb\n";
    char expected[] = "a\n\nb\n";
    char buf[256];

    struct Options opts = {0};
    opts.squeeze_blank = true;
    struct CatState state = {1, true, false};

    mu_assert_inteq(0, RunFileProcessing(input, &opts, &state, buf, sizeof(buf)));
    mu_assert_streq(expected, buf);

    return 0;
}

// -s: одиночная пустая строка остаётся
static int test_squeeze_keeps_single(void) {
    char input[] = "a\n\nb\n";
    char expected[] = "a\n\nb\n";
    char buf[256];

    struct Options opts = {0};
    opts.squeeze_blank = true;
    struct CatState state = {1, true, false};

    mu_assert_inteq(0, RunFileProcessing(input, &opts, &state, buf, sizeof(buf)));
    mu_assert_streq(expected, buf);

    return 0;
}

// -s: пустые строки в начале тоже схлопываются
static int test_squeeze_leading_blanks(void) {
    char input[] = "\n\n\na\n";
    char expected[] = "\na\n";
    char buf[256];

    struct Options opts = {0};
    opts.squeeze_blank = true;
    struct CatState state = {1, true, false};

    mu_assert_inteq(0, RunFileProcessing(input, &opts, &state, buf, sizeof(buf)));
    mu_assert_streq(expected, buf);

    return 0;
}

// -E: $ перед каждым переводом строки
static int test_show_ends(void) {
    char input[] = "a\nb\n";
    char expected[] = "a$\nb$\n";
    char buf[256];

    struct Options opts = {0};
    opts.show_ends = true;
    struct CatState state = {1, true, false};

    mu_assert_inteq(0, RunFileProcessing(input, &opts, &state, buf, sizeof(buf)));
    mu_assert_streq(expected, buf);

    return 0;
}

// -E: без перевода строки в конце $ не добавляется
static int test_show_ends_no_trailing_newline(void) {
    char input[] = "a\nb";
    char expected[] = "a$\nb";
    char buf[256];

    struct Options opts = {0};
    opts.show_ends = true;
    struct CatState state = {1, true, false};

    mu_assert_inteq(0, RunFileProcessing(input, &opts, &state, buf, sizeof(buf)));
    mu_assert_streq(expected, buf);

    return 0;
}

// -T: табуляция выводится как ^I
static int test_show_tabs(void) {
    char input[] = "a\tb\n";
    char expected[] = "a^Ib\n";
    char buf[256];

    struct Options opts = {0};
    opts.show_tabs = true;
    struct CatState state = {1, true, false};

    mu_assert_inteq(0, RunFileProcessing(input, &opts, &state, buf, sizeof(buf)));
    mu_assert_streq(expected, buf);

    return 0;
}

// -T не касается табуляции в префиксе номера
static int test_number_tab_not_converted(void) {
    char input[] = "a\tb\n";
    char expected[] = "     1\ta^Ib\n";
    char buf[256];

    struct Options opts = {0};
    opts.number_lines = true;
    opts.show_tabs = true;
    struct CatState state = {1, true, false};

    mu_assert_inteq(0, RunFileProcessing(input, &opts, &state, buf, sizeof(buf)));
    mu_assert_streq(expected, buf);

    return 0;
}

// -nET вместе: номер, ^I и $ в одной строке
static int test_all_flags_together(void) {
    char input[] = "a\tb\n\n";
    char expected[] = "     1\ta^Ib$\n     2\t$\n";
    char buf[256];

    struct Options opts = {0};
    opts.number_lines = true;
    opts.show_ends = true;
    opts.show_tabs = true;
    struct CatState state = {1, true, false};

    mu_assert_inteq(0, RunFileProcessing(input, &opts, &state, buf, sizeof(buf)));
    mu_assert_streq(expected, buf);

    return 0;
}

// -n: после обработки счётчик указывает на следующую строку
static int test_number_updates_state(void) {
    char input[] = "a\nb\n";
    char buf[256];

    struct Options opts = {0};
    opts.number_lines = true;
    struct CatState state = {1, true, false};

    mu_assert_inteq(0, RunFileProcessing(input, &opts, &state, buf, sizeof(buf)));
    mu_assert_inteq(3, state.line_number);  // две строки пронумерованы, следующая — третья

    return 0;
}

// -n: нумерация сквозная через вызовы, как через несколько файлов
static int test_number_continues_across_calls(void) {
    char first[] = "a\n";
    char second[] = "b\n";
    char buf[256];

    struct Options opts = {0};
    opts.number_lines = true;
    struct CatState state = {1, true, false};

    mu_assert_inteq(0, RunFileProcessing(first, &opts, &state, buf, sizeof(buf)));
    mu_assert_streq("     1\ta\n", buf);

    mu_assert_inteq(0, RunFileProcessing(second, &opts, &state, buf, sizeof(buf)));
    mu_assert_streq("     2\tb\n", buf);  // счёт продолжился, а не начался заново

    return 0;
}

// -n: файл без перевода строки в конце — следующий вызов продолжает ту же строку
static int test_unfinished_line_continues(void) {
    char first[] = "x";
    char second[] = "y\nz\n";
    char buf[256];

    struct Options opts = {0};
    opts.number_lines = true;
    struct CatState state = {1, true, false};

    mu_assert_inteq(0, RunFileProcessing(first, &opts, &state, buf, sizeof(buf)));
    mu_assert_streq("     1\tx", buf);

    mu_assert_inteq(0, RunFileProcessing(second, &opts, &state, buf, sizeof(buf)));
    mu_assert_streq("y\n     2\tz\n", buf);  // номер только после настоящего \n

    return 0;
}
