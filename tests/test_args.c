#define _POSIX_C_SOURCE 200809L  // нужен для dup/dup2

#include <unistd.h>

#include "../src/args.h"
#include "../src/my_errors.h"
#include "minunit.h"

// Вторая строка сообщения об ошибке, ожидаемая от ParseFlags
#define HINT "Try 'cat --help' for more information.\n"

// Метка, которую PrintHelp не должна затирать
#define MARK "<<<\n"

static int test_null_options(void);
static int test_dash_options(void);
static int test_two_dash_options(void);
static int test_one_file_name_options(void);
static int test_two_file_names_options(void);
static int test_dushes_file_names_options(void);
static int test_file_names_dushes_options(void);

static int test_files1_and_options(void);
static int test_files2_and_options(void);
static int test_files3_and_options(void);
static int test_files4_and_options(void);
static int test_files5_and_options(void);
static int test_files6_and_options(void);
static int test_files7_and_options(void);
static int test_files8_and_options(void);
static int test_files9_and_options(void);
static int test_files10_and_options(void);
static int test_files11_and_options(void);
static int test_files12_and_options(void);
static int test_files13_and_options(void);
static int test_files14_and_options(void);
static int test_files15_and_options(void);
static int test_files16_and_options(void);
static int test_files17_and_options(void);
static int test_files18_and_options(void);

static int test_open_input_dash(void);
static int test_open_input_file(void);
static int test_open_input_missing(void);
static int test_close_input_keeps_stdin(void);
static int test_close_input_clears_eof(void);
static int test_close_input_closes_file(void);

static int test_options_free_clears_pointer(void);
static int test_options_free_twice(void);
static int test_options_free_zeroed(void);

static int test_flags_unknown_short(void);
static int test_flags_unknown_long(void);
static int test_flags_single_dash_word(void);
static int test_flags_double_dash(void);
static int test_flags_valid_silent(void);
static int test_flags_help_silent(void);
static int test_flags_stops_after_error(void);
static int test_flags_keeps_parsed_before_error(void);

static int test_help_writes_to_stream(void);
static int test_help_header(void);
static int test_help_lists_all_flags(void);
static int test_help_has_no_unimplemented_flags(void);
static int test_help_ends_with_newline(void);
static int test_help_is_repeatable(void);

static int test_all1_options(void);
static int test_all2_options(void);
static int test_all3_options(void);
static int test_all4_options(void);
static int test_all5_options(void);
static int test_all6_options(void);
static int test_all7_options(void);
static int test_all8_options(void);
static int test_all9_options(void);

int main(void) {
    mu_run_test(test_null_options);
    mu_run_test(test_dash_options);
    mu_run_test(test_two_dash_options);
    mu_run_test(test_one_file_name_options);
    mu_run_test(test_two_file_names_options);
    mu_run_test(test_dushes_file_names_options);
    mu_run_test(test_file_names_dushes_options);

    mu_run_test(test_files1_and_options);
    mu_run_test(test_files2_and_options);
    mu_run_test(test_files3_and_options);
    mu_run_test(test_files4_and_options);
    mu_run_test(test_files5_and_options);
    mu_run_test(test_files6_and_options);
    mu_run_test(test_files7_and_options);
    mu_run_test(test_files8_and_options);
    mu_run_test(test_files9_and_options);
    mu_run_test(test_files10_and_options);
    mu_run_test(test_files11_and_options);
    mu_run_test(test_files12_and_options);
    mu_run_test(test_files13_and_options);
    mu_run_test(test_files14_and_options);
    mu_run_test(test_files15_and_options);
    mu_run_test(test_files16_and_options);
    mu_run_test(test_files17_and_options);
    mu_run_test(test_files18_and_options);

    mu_run_test(test_open_input_dash);
    mu_run_test(test_open_input_file);
    mu_run_test(test_open_input_missing);
    mu_run_test(test_close_input_keeps_stdin);
    mu_run_test(test_close_input_clears_eof);
    mu_run_test(test_close_input_closes_file);

    mu_run_test(test_options_free_clears_pointer);
    mu_run_test(test_options_free_twice);
    mu_run_test(test_options_free_zeroed);

    mu_run_test(test_flags_unknown_short);
    mu_run_test(test_flags_unknown_long);
    mu_run_test(test_flags_single_dash_word);
    mu_run_test(test_flags_double_dash);
    mu_run_test(test_flags_valid_silent);
    mu_run_test(test_flags_help_silent);
    mu_run_test(test_flags_stops_after_error);
    mu_run_test(test_flags_keeps_parsed_before_error);

    mu_run_test(test_help_writes_to_stream);
    mu_run_test(test_help_header);
    mu_run_test(test_help_lists_all_flags);
    mu_run_test(test_help_has_no_unimplemented_flags);
    mu_run_test(test_help_ends_with_newline);
    mu_run_test(test_help_is_repeatable);

    mu_run_test(test_all1_options);
    mu_run_test(test_all2_options);
    mu_run_test(test_all3_options);
    mu_run_test(test_all4_options);
    mu_run_test(test_all5_options);
    mu_run_test(test_all6_options);
    mu_run_test(test_all7_options);
    mu_run_test(test_all8_options);
    mu_run_test(test_all9_options);

    mu_report();
    return mu_tests_failed != 0;
}

// Тесты для ParseFlags
//
// Установку флагов проверяют тесты ParseArgs выше, здесь — вывод сообщений
// об ошибках: stderr перенаправляется во временный файл и читается обратно.

// Перенаправляет stderr во временный файл, вызывает ParseFlags и возвращает её код.
// Текст, попавший в stderr, кладёт в buf. stderr восстанавливается.
static int ParseFlagsStderr(const char *arg, struct Options *opts, char *buf, size_t size) {
    const char *tmp_path = "build/stderr.tmp";
    int res = -1;

    buf[0] = '\0';

    int saved = dup(STDERR_FILENO);  // запомнить настоящий stderr
    if (saved != -1 && freopen(tmp_path, "w", stderr) != NULL) {
        res = ParseFlags(arg, opts);
        fflush(stderr);  // после freopen поток буферизован, без сброса файл пуст

        FILE *tmp = fopen(tmp_path, "r");
        if (tmp != NULL) {
            size_t len = fread(buf, 1, size - 1, tmp);
            buf[len] = '\0';
            fclose(tmp);
        }

        dup2(saved, STDERR_FILENO);  // вернуть stderr на место
        clearerr(stderr);
    }

    if (saved != -1) close(saved);
    remove(tmp_path);

    return res;
}

// 8. Флаги, разобранные до ошибки, остаются взведёнными
static int test_flags_keeps_parsed_before_error(void) {
    struct Options opts = {0};
    char buf[256];

    int res = ParseFlagsStderr("-nz", &opts, buf, sizeof(buf));
    int res_expected = UNKNOWN_FLAG_ERROR;

    mu_assert_inteq(res_expected, res);  // Проверка кода возврата ParseFlags
    mu_assert_streq("cat: invalid option -- 'z'\n" HINT, buf);  // Проверка сообщения про 'z'
    mu_assert("-n before the error must stay set", opts.number_lines);  // Проверка что -n взведён

    return 0;
}

// 7. Разбор прекращается на первом неизвестном символе: одно сообщение, остальное не читается
static int test_flags_stops_after_error(void) {
    struct Options opts = {0};
    char buf[256];

    int res = ParseFlagsStderr("-zn", &opts, buf, sizeof(buf));
    int res_expected = UNKNOWN_FLAG_ERROR;

    mu_assert_inteq(res_expected, res);  // Проверка кода возврата ParseFlags
    mu_assert_streq("cat: invalid option -- 'z'\n" HINT, buf);  // Проверка что сообщение ровно одно
    mu_assert("-n after the error must not be set",
              !opts.number_lines);  // Проверка что разбор прерван

    return 0;
}

// 6. --help ничего не печатает в stderr и даёт HELP_REQUESTED
static int test_flags_help_silent(void) {
    struct Options opts = {0};
    char buf[256];

    int res = ParseFlagsStderr("--help", &opts, buf, sizeof(buf));
    int res_expected = HELP_REQUESTED;  // не ошибка, но разбор прекращается

    mu_assert_inteq(res_expected, res);          // Проверка кода возврата ParseFlags
    mu_assert_streq("", buf);                    // Проверка что stderr пуст
    mu_assert("--help must be set", opts.help);  // Проверка что --help взведён

    return 0;
}

// 5. Успешный разбор ничего не печатает
static int test_flags_valid_silent(void) {
    struct Options opts = {0};
    char buf[256];

    int res = ParseFlagsStderr("-nE", &opts, buf, sizeof(buf));
    int res_expected = SUCCESS;

    mu_assert_inteq(res_expected, res);              // Проверка кода возврата ParseFlags
    mu_assert_streq("", buf);                        // Проверка что stderr пуст
    mu_assert("-n must be set", opts.number_lines);  // Проверка что -n взведён
    mu_assert("-E must be set", opts.show_ends);     // Проверка что -E взведён

    return 0;
}

// 4. "--" не поддерживается (см. «Границы» в плане) и разбирается как длинный флаг
static int test_flags_double_dash(void) {
    struct Options opts = {0};
    char buf[256];

    int res = ParseFlagsStderr("--", &opts, buf, sizeof(buf));
    int res_expected = UNKNOWN_FLAG_ERROR;

    mu_assert_inteq(res_expected, res);  // Проверка кода возврата ParseFlags
    mu_assert_streq("cat: unrecognized option '--'\n" HINT, buf);  // Проверка сообщения

    return 0;
}

// 3. "-help" — одиночный дефис, значит посимвольный разбор, а не длинный флаг
static int test_flags_single_dash_word(void) {
    struct Options opts = {0};
    char buf[256];

    int res = ParseFlagsStderr("-help", &opts, buf, sizeof(buf));
    int res_expected = UNKNOWN_FLAG_ERROR;

    mu_assert_inteq(res_expected, res);  // Проверка кода возврата ParseFlags
    mu_assert_streq("cat: invalid option -- 'h'\n" HINT, buf);  // Проверка сообщения про символ
    mu_assert("--help must not be set", !opts.help);  // Проверка что это не спутано с --help

    return 0;
}

// 2. Неизвестный длинный флаг: в сообщении вся строка, а не символ
static int test_flags_unknown_long(void) {
    struct Options opts = {0};
    char buf[256];

    int res = ParseFlagsStderr("--number", &opts, buf, sizeof(buf));
    int res_expected = UNKNOWN_FLAG_ERROR;

    mu_assert_inteq(res_expected, res);  // Проверка кода возврата ParseFlags
    mu_assert_streq("cat: unrecognized option '--number'\n" HINT, buf);  // Проверка сообщения

    return 0;
}

// 1. Неизвестный короткий флаг: в сообщении символ
static int test_flags_unknown_short(void) {
    struct Options opts = {0};
    char buf[256];

    int res = ParseFlagsStderr("-z", &opts, buf, sizeof(buf));
    int res_expected = UNKNOWN_FLAG_ERROR;

    mu_assert_inteq(res_expected, res);  // Проверка кода возврата ParseFlags
    mu_assert_streq("cat: invalid option -- 'z'\n" HINT, buf);  // Проверка сообщения

    return 0;
}

// Тесты для OptionsFree

// 3. OptionsFree на нетронутой структуре: free(NULL) законен, падать нечему
static int test_options_free_zeroed(void) {
    struct Options opts = {0};  // ParseArgs не вызывался, память не выделялась

    OptionsFree(&opts);

    int expected_file_count = 0;

    mu_assert("files must stay NULL", opts.files == NULL);  // Проверка что указатель пуст
    mu_assert_inteq(expected_file_count, opts.file_count);  // Проверка что счётчик файлов сброшен

    return 0;
}

// 2. Повторный вызов OptionsFree безопасен: второй free получает уже NULL
static int test_options_free_twice(void) {
    struct Options opts = {0};

    char *argv[] = {"file_name", "a.txt", NULL};
    int argc = sizeof(argv) / sizeof(argv[0]) - 1;  // NULL в argc не входит

    int res = ParseArgs(argc, argv, &opts);
    int res_expected = 0;

    mu_assert_inteq(res_expected, res);  // Проверка кода возврата ParseArgs
    mu_assert("Can't allocate memory", opts.files != NULL);  // Проверка что память выделена

    OptionsFree(&opts);
    OptionsFree(&opts);  // второй вызов не должен приводить к двойному освобождению

    int expected_file_count = 0;

    mu_assert("files must be NULL after double free",
              opts.files == NULL);  // Проверка что указатель пуст
    mu_assert_inteq(expected_file_count, opts.file_count);  // Проверка что счётчик файлов сброшен

    return 0;
}

// 1. После OptionsFree структура согласована: указатель обнулён, счётчик сброшен
static int test_options_free_clears_pointer(void) {
    struct Options opts = {0};

    char *argv[] = {"file_name", "a.txt", "b.txt", NULL};
    int argc = sizeof(argv) / sizeof(argv[0]) - 1;  // NULL в argc не входит

    int res = ParseArgs(argc, argv, &opts);
    int res_expected = 0;

    mu_assert_inteq(res_expected, res);  // Проверка кода возврата ParseArgs
    mu_assert("Can't allocate memory", opts.files != NULL);  // Проверка что память выделена

    OptionsFree(&opts);

    int expected_file_count = 0;

    mu_assert("files must be NULL after free", opts.files == NULL);  // Проверка что указатель пуст
    mu_assert_inteq(expected_file_count, opts.file_count);  // Проверка что счётчик файлов сброшен

    return 0;
}

// Тесты для OpenInput и CloseInput
//
// Тесты читают файлы из tests/data, поэтому запускать их нужно из корня проекта
// (make test так и делает).

// 6. CloseInput действительно закрывает обычный файл, иначе кончатся дескрипторы
static int test_close_input_closes_file(void) {
    FILE *in = NULL;

    for (int i = 0; i < 2000; i++) {  // предел дескрипторов обычно 1024
        in = OpenInput("tests/data/single_line.txt");
        if (in == NULL) break;
        CloseInput(in);
    }

    mu_assert("Descriptors leaked: CloseInput doesn't close files",
              in != NULL);  // Проверка что файлы закрываются

    return 0;
}

// 5. CloseInput сбрасывает признак EOF у stdin, иначе второй "-" ничего не прочитает
static int test_close_input_clears_eof(void) {
    const FILE *in = freopen("tests/data/single_line.txt", "r", stdin);

    mu_assert("Can't redirect stdin", in != NULL);

    while (fgetc(stdin) != EOF) {  // дочитали stdin до конца
    }

    mu_assert("EOF must be set after reading to the end",
              feof(stdin) != 0);  // Проверка что признак EOF взведён

    CloseInput(stdin);

    mu_assert("CloseInput must clear EOF on stdin",
              feof(stdin) == 0);  // Проверка что признак EOF сброшен

    return 0;
}

// 4. CloseInput не закрывает stdin: после неё поток читается дальше
static int test_close_input_keeps_stdin(void) {
    const FILE *in = freopen("tests/data/single_line.txt", "r", stdin);

    mu_assert("Can't redirect stdin", in != NULL);

    int first = fgetc(stdin);
    CloseInput(stdin);
    int second = fgetc(stdin);

    mu_assert_inteq('h', first);  // Проверка что прочитан первый символ файла
    mu_assert_inteq('e', second);  // Проверка что поток жив и чтение продолжилось

    return 0;
}

// 3. OpenInput на несуществующем файле возвращает NULL
static int test_open_input_missing(void) {
    const FILE *in = OpenInput("tests/data/nope.txt");

    mu_assert("Missing file must give NULL", in == NULL);  // Проверка что вернулся NULL

    return 0;
}

// 2. OpenInput открывает существующий файл и читает его с начала
static int test_open_input_file(void) {
    FILE *in = OpenInput("tests/data/single_line.txt");

    mu_assert("Can't open existing file", in != NULL);  // Проверка что файл открылся

    char expected_content[] = "hello\n";
    char buf[16];
    size_t size = fread(buf, 1, sizeof(buf) - 1, in);
    buf[size] = '\0';

    fclose(in);

    mu_assert_streq(expected_content, buf);  // Проверка что содержимое файла совпадает

    return 0;
}

// 1. OpenInput("-") возвращает именно stdin, а не открывает файл с таким именем
static int test_open_input_dash(void) {
    const FILE *in = OpenInput("-");

    mu_assert("OpenInput(\"-\") must return stdin", in == stdin);  // Проверка что вернулся stdin

    return 0;
}

// Тесты для файлов и опций

// 18. -- не поддерживается (см. «Границы» в плане)
static int test_files18_and_options(void) {
    struct Options opts = {0};

    char *argv[] = {"file_name", "--", "a.txt", NULL};
    int argc = sizeof(argv) / sizeof(argv[0]) - 1;  // NULL в argc не входит

    bool expected_number_lines = false;     // -n
    bool expected_number_nonblank = false;  // -b
    bool expected_squeeze_blank = false;    // -s
    bool expected_show_ends = false;        // -E
    bool expected_show_tabs = false;        // -T
    bool expected_help = false;             // --help

    int res = ParseArgs(argc, argv, &opts);
    int res_expected = 2;  // ParseArgs упала при разборе

    mu_assert_inteq(res_expected, res);  // Проверка кода возврата ParseArgs
    mu_assert_inteq(expected_number_lines, opts.number_lines);        // -n отработан верно
    mu_assert_inteq(expected_number_nonblank, opts.number_nonblank);  // -b отработан верно
    mu_assert_inteq(expected_squeeze_blank, opts.squeeze_blank);      // -s отработан верно
    mu_assert_inteq(expected_show_ends, opts.show_ends);              // -E отработан верно
    mu_assert_inteq(expected_show_tabs, opts.show_tabs);              // -T отработан верно
    mu_assert_inteq(expected_help, opts.help);                        // --help отработан верно

    // При ошибке разбора список файлов не определён, поэтому не проверяется

    OptionsFree(&opts);
    return 0;
}

// 17. пустая строка как имя файла
static int test_files17_and_options(void) {
    struct Options opts = {0};

    char *argv[] = {"file_name", "", NULL};
    int argc = sizeof(argv) / sizeof(argv[0]) - 1;  // NULL в argc не входит

    bool expected_number_lines = false;     // -n
    bool expected_number_nonblank = false;  // -b
    bool expected_squeeze_blank = false;    // -s
    bool expected_show_ends = false;        // -E
    bool expected_show_tabs = false;        // -T
    bool expected_help = false;             // --help

    char *expected_argv[] = {""};
    int expected_file_count = sizeof(expected_argv) / sizeof(expected_argv[0]);

    int res = ParseArgs(argc, argv, &opts);
    int res_expected = 0;

    mu_assert_inteq(res_expected, res);  // Проверка кода возврата ParseArgs
    mu_assert_inteq(expected_number_lines, opts.number_lines);        // -n отработан верно
    mu_assert_inteq(expected_number_nonblank, opts.number_nonblank);  // -b отработан верно
    mu_assert_inteq(expected_squeeze_blank, opts.squeeze_blank);      // -s отработан верно
    mu_assert_inteq(expected_show_ends, opts.show_ends);              // -E отработан верно
    mu_assert_inteq(expected_show_tabs, opts.show_tabs);              // -T отработан верно
    mu_assert_inteq(expected_help, opts.help);                        // --help отработан верно

    mu_assert("Can't allocate memory", opts.files != NULL);  // Проверка что память выделена
    mu_assert("Сount out of range",
              opts.file_count > 0 &&
                  opts.file_count <= argc);  // Проверка что количество файлов в правильном диапазоне
    mu_assert_inteq(expected_file_count,
                    opts.file_count);  // Проверка что количество файлов считает правильно
    for (int i = 0; i < expected_file_count; i++) {
        mu_assert_streq(expected_argv[i], opts.files[i]);  // Проверка что имена файлов совпадают
    }

    OptionsFree(&opts);
    return 0;
}

// 16. длинный флаг не из списка
static int test_files16_and_options(void) {
    struct Options opts = {0};

    char *argv[] = {"file_name", "--number", NULL};
    int argc = sizeof(argv) / sizeof(argv[0]) - 1;  // NULL в argc не входит

    bool expected_number_lines = false;     // -n
    bool expected_number_nonblank = false;  // -b
    bool expected_squeeze_blank = false;    // -s
    bool expected_show_ends = false;        // -E
    bool expected_show_tabs = false;        // -T
    bool expected_help = false;             // --help

    int res = ParseArgs(argc, argv, &opts);
    int res_expected = 2;  // ParseArgs упала при разборе

    mu_assert_inteq(res_expected, res);  // Проверка кода возврата ParseArgs
    mu_assert_inteq(expected_number_lines, opts.number_lines);        // -n отработан верно
    mu_assert_inteq(expected_number_nonblank, opts.number_nonblank);  // -b отработан верно
    mu_assert_inteq(expected_squeeze_blank, opts.squeeze_blank);      // -s отработан верно
    mu_assert_inteq(expected_show_ends, opts.show_ends);              // -E отработан верно
    mu_assert_inteq(expected_show_tabs, opts.show_tabs);              // -T отработан верно
    mu_assert_inteq(expected_help, opts.help);                        // --help отработан верно

    // При ошибке разбора список файлов не определён, поэтому не проверяется

    OptionsFree(&opts);
    return 0;
}

// 15. -help — одиночный дефис, а не длинный флаг
static int test_files15_and_options(void) {
    struct Options opts = {0};

    char *argv[] = {"file_name", "-help", "a.txt", NULL};
    int argc = sizeof(argv) / sizeof(argv[0]) - 1;  // NULL в argc не входит

    bool expected_number_lines = false;     // -n
    bool expected_number_nonblank = false;  // -b
    bool expected_squeeze_blank = false;    // -s
    bool expected_show_ends = false;        // -E
    bool expected_show_tabs = false;        // -T
    bool expected_help = false;             // --help

    int res = ParseArgs(argc, argv, &opts);
    int res_expected = 2;  // ParseArgs упала при разборе

    mu_assert_inteq(res_expected, res);  // Проверка кода возврата ParseArgs
    mu_assert_inteq(expected_number_lines, opts.number_lines);        // -n отработан верно
    mu_assert_inteq(expected_number_nonblank, opts.number_nonblank);  // -b отработан верно
    mu_assert_inteq(expected_squeeze_blank, opts.squeeze_blank);      // -s отработан верно
    mu_assert_inteq(expected_show_ends, opts.show_ends);              // -E отработан верно
    mu_assert_inteq(expected_show_tabs, opts.show_tabs);              // -T отработан верно
    mu_assert_inteq(expected_help, opts.help);                        // --help отработан верно

    // При ошибке разбора список файлов не определён, поэтому не проверяется

    OptionsFree(&opts);
    return 0;
}

// 14. повторённый флаг в склейке
static int test_files14_and_options(void) {
    struct Options opts = {0};

    char *argv[] = {"file_name", "-nn", "a.txt", NULL};
    int argc = sizeof(argv) / sizeof(argv[0]) - 1;  // NULL в argc не входит

    bool expected_number_lines = true;      // -n
    bool expected_number_nonblank = false;  // -b
    bool expected_squeeze_blank = false;    // -s
    bool expected_show_ends = false;        // -E
    bool expected_show_tabs = false;        // -T
    bool expected_help = false;             // --help

    char *expected_argv[] = {"a.txt"};
    int expected_file_count = sizeof(expected_argv) / sizeof(expected_argv[0]);

    int res = ParseArgs(argc, argv, &opts);
    int res_expected = 0;

    mu_assert_inteq(res_expected, res);  // Проверка кода возврата ParseArgs
    mu_assert_inteq(expected_number_lines, opts.number_lines);        // -n отработан верно
    mu_assert_inteq(expected_number_nonblank, opts.number_nonblank);  // -b отработан верно
    mu_assert_inteq(expected_squeeze_blank, opts.squeeze_blank);      // -s отработан верно
    mu_assert_inteq(expected_show_ends, opts.show_ends);              // -E отработан верно
    mu_assert_inteq(expected_show_tabs, opts.show_tabs);              // -T отработан верно
    mu_assert_inteq(expected_help, opts.help);                        // --help отработан верно

    mu_assert("Can't allocate memory", opts.files != NULL);  // Проверка что память выделена
    mu_assert("Сount out of range",
              opts.file_count > 0 &&
                  opts.file_count <= argc);  // Проверка что количество файлов в правильном диапазоне
    mu_assert_inteq(expected_file_count,
                    opts.file_count);  // Проверка что количество файлов считает правильно
    for (int i = 0; i < expected_file_count; i++) {
        mu_assert_streq(expected_argv[i], opts.files[i]);  // Проверка что имена файлов совпадают
    }

    OptionsFree(&opts);
    return 0;
}

// 13. повторённый флаг отдельными аргументами
static int test_files13_and_options(void) {
    struct Options opts = {0};

    char *argv[] = {"file_name", "-n", "-n", "a.txt", NULL};
    int argc = sizeof(argv) / sizeof(argv[0]) - 1;  // NULL в argc не входит

    bool expected_number_lines = true;      // -n
    bool expected_number_nonblank = false;  // -b
    bool expected_squeeze_blank = false;    // -s
    bool expected_show_ends = false;        // -E
    bool expected_show_tabs = false;        // -T
    bool expected_help = false;             // --help

    char *expected_argv[] = {"a.txt"};
    int expected_file_count = sizeof(expected_argv) / sizeof(expected_argv[0]);

    int res = ParseArgs(argc, argv, &opts);
    int res_expected = 0;

    mu_assert_inteq(res_expected, res);  // Проверка кода возврата ParseArgs
    mu_assert_inteq(expected_number_lines, opts.number_lines);        // -n отработан верно
    mu_assert_inteq(expected_number_nonblank, opts.number_nonblank);  // -b отработан верно
    mu_assert_inteq(expected_squeeze_blank, opts.squeeze_blank);      // -s отработан верно
    mu_assert_inteq(expected_show_ends, opts.show_ends);              // -E отработан верно
    mu_assert_inteq(expected_show_tabs, opts.show_tabs);              // -T отработан верно
    mu_assert_inteq(expected_help, opts.help);                        // --help отработан верно

    mu_assert("Can't allocate memory", opts.files != NULL);  // Проверка что память выделена
    mu_assert("Сount out of range",
              opts.file_count > 0 &&
                  opts.file_count <= argc);  // Проверка что количество файлов в правильном диапазоне
    mu_assert_inteq(expected_file_count,
                    opts.file_count);  // Проверка что количество файлов считает правильно
    for (int i = 0; i < expected_file_count; i++) {
        mu_assert_streq(expected_argv[i], opts.files[i]);  // Проверка что имена файлов совпадают
    }

    OptionsFree(&opts);
    return 0;
}

// 12. мусор в склеенных флагах
static int test_files12_and_options(void) {
    struct Options opts = {0};

    char *argv[] = {"file_name", "-n2", "a.txt", NULL};
    int argc = sizeof(argv) / sizeof(argv[0]) - 1;  // NULL в argc не входит

    bool expected_number_lines = true;      // -n
    bool expected_number_nonblank = false;  // -b
    bool expected_squeeze_blank = false;    // -s
    bool expected_show_ends = false;        // -E
    bool expected_show_tabs = false;        // -T
    bool expected_help = false;             // --help

    int res = ParseArgs(argc, argv, &opts);
    int res_expected = 2;  // ParseArgs упала при разборе

    mu_assert_inteq(res_expected, res);  // Проверка кода возврата ParseArgs
    mu_assert_inteq(expected_number_lines, opts.number_lines);        // -n отработан верно
    mu_assert_inteq(expected_number_nonblank, opts.number_nonblank);  // -b отработан верно
    mu_assert_inteq(expected_squeeze_blank, opts.squeeze_blank);      // -s отработан верно
    mu_assert_inteq(expected_show_ends, opts.show_ends);              // -E отработан верно
    mu_assert_inteq(expected_show_tabs, opts.show_tabs);              // -T отработан верно
    mu_assert_inteq(expected_help, opts.help);                        // --help отработан верно

    // При ошибке разбора список файлов не определён, поэтому не проверяется

    OptionsFree(&opts);
    return 0;
}

// 11. неизвестный флаг среди файлов
static int test_files11_and_options(void) {
    struct Options opts = {0};

    char *argv[] = {"file_name", "a.txt", "-z", "b.txt", NULL};
    int argc = sizeof(argv) / sizeof(argv[0]) - 1;  // NULL в argc не входит

    bool expected_number_lines = false;     // -n
    bool expected_number_nonblank = false;  // -b
    bool expected_squeeze_blank = false;    // -s
    bool expected_show_ends = false;        // -E
    bool expected_show_tabs = false;        // -T
    bool expected_help = false;             // --help

    int res = ParseArgs(argc, argv, &opts);
    int res_expected = 2;  // ParseArgs упала при разборе

    mu_assert_inteq(res_expected, res);  // Проверка кода возврата ParseArgs
    mu_assert_inteq(expected_number_lines, opts.number_lines);        // -n отработан верно
    mu_assert_inteq(expected_number_nonblank, opts.number_nonblank);  // -b отработан верно
    mu_assert_inteq(expected_squeeze_blank, opts.squeeze_blank);      // -s отработан верно
    mu_assert_inteq(expected_show_ends, opts.show_ends);              // -E отработан верно
    mu_assert_inteq(expected_show_tabs, opts.show_tabs);              // -T отработан верно
    mu_assert_inteq(expected_help, opts.help);                        // --help отработан верно

    // При ошибке разбора список файлов не определён, поэтому не проверяется

    OptionsFree(&opts);
    return 0;
}

// 10. --help вместе с файлом: разбор прекращается, до файла дело не доходит
static int test_files10_and_options(void) {
    struct Options opts = {0};

    char *argv[] = {"file_name", "--help", "a.txt", NULL};
    int argc = sizeof(argv) / sizeof(argv[0]) - 1;  // NULL в argc не входит

    bool expected_number_lines = false;     // -n
    bool expected_number_nonblank = false;  // -b
    bool expected_squeeze_blank = false;    // -s
    bool expected_show_ends = false;        // -E
    bool expected_show_tabs = false;        // -T
    bool expected_help = true;              // --help

    int expected_file_count = 0;  // разбор прекращён на --help, файл не добавлен

    int res = ParseArgs(argc, argv, &opts);
    int res_expected = HELP_REQUESTED;

    mu_assert_inteq(res_expected, res);  // Проверка кода возврата ParseArgs
    mu_assert_inteq(expected_number_lines, opts.number_lines);        // -n отработан верно
    mu_assert_inteq(expected_number_nonblank, opts.number_nonblank);  // -b отработан верно
    mu_assert_inteq(expected_squeeze_blank, opts.squeeze_blank);      // -s отработан верно
    mu_assert_inteq(expected_show_ends, opts.show_ends);              // -E отработан верно
    mu_assert_inteq(expected_show_tabs, opts.show_tabs);              // -T отработан верно
    mu_assert_inteq(expected_help, opts.help);                        // --help отработан верно

    mu_assert("Can't allocate memory", opts.files != NULL);  // Проверка что память выделена
    mu_assert_inteq(expected_file_count, opts.file_count);  // Проверка что файлы не разбирались

    OptionsFree(&opts);
    return 0;
}

// 9. только флаги, файлов нет
static int test_files9_and_options(void) {
    struct Options opts = {0};

    char *argv[] = {"file_name", "-n", NULL};
    int argc = sizeof(argv) / sizeof(argv[0]) - 1;  // NULL в argc не входит

    bool expected_number_lines = true;      // -n
    bool expected_number_nonblank = false;  // -b
    bool expected_squeeze_blank = false;    // -s
    bool expected_show_ends = false;        // -E
    bool expected_show_tabs = false;        // -T
    bool expected_help = false;             // --help

    char *expected_argv[] = {"-"};
    int expected_file_count = sizeof(expected_argv) / sizeof(expected_argv[0]);

    int res = ParseArgs(argc, argv, &opts);
    int res_expected = 0;

    mu_assert_inteq(res_expected, res);  // Проверка кода возврата ParseArgs
    mu_assert_inteq(expected_number_lines, opts.number_lines);        // -n отработан верно
    mu_assert_inteq(expected_number_nonblank, opts.number_nonblank);  // -b отработан верно
    mu_assert_inteq(expected_squeeze_blank, opts.squeeze_blank);      // -s отработан верно
    mu_assert_inteq(expected_show_ends, opts.show_ends);              // -E отработан верно
    mu_assert_inteq(expected_show_tabs, opts.show_tabs);              // -T отработан верно
    mu_assert_inteq(expected_help, opts.help);                        // --help отработан верно

    mu_assert("Can't allocate memory", opts.files != NULL);  // Проверка что память выделена
    mu_assert("Сount out of range",
              opts.file_count > 0 &&
                  opts.file_count <= argc);  // Проверка что количество файлов в правильном диапазоне
    mu_assert_inteq(expected_file_count,
                    opts.file_count);  // Проверка что количество файлов считает правильно
    for (int i = 0; i < expected_file_count; i++) {
        mu_assert_streq(expected_argv[i], opts.files[i]);  // Проверка что имена файлов совпадают
    }

    OptionsFree(&opts);
    return 0;
}

// 8. stdin перед флагом
static int test_files8_and_options(void) {
    struct Options opts = {0};

    char *argv[] = {"file_name", "-", "-n", NULL};
    int argc = sizeof(argv) / sizeof(argv[0]) - 1;  // NULL в argc не входит

    bool expected_number_lines = true;      // -n
    bool expected_number_nonblank = false;  // -b
    bool expected_squeeze_blank = false;    // -s
    bool expected_show_ends = false;        // -E
    bool expected_show_tabs = false;        // -T
    bool expected_help = false;             // --help

    char *expected_argv[] = {"-"};
    int expected_file_count = sizeof(expected_argv) / sizeof(expected_argv[0]);

    int res = ParseArgs(argc, argv, &opts);
    int res_expected = 0;

    mu_assert_inteq(res_expected, res);  // Проверка кода возврата ParseArgs
    mu_assert_inteq(expected_number_lines, opts.number_lines);        // -n отработан верно
    mu_assert_inteq(expected_number_nonblank, opts.number_nonblank);  // -b отработан верно
    mu_assert_inteq(expected_squeeze_blank, opts.squeeze_blank);      // -s отработан верно
    mu_assert_inteq(expected_show_ends, opts.show_ends);              // -E отработан верно
    mu_assert_inteq(expected_show_tabs, opts.show_tabs);              // -T отработан верно
    mu_assert_inteq(expected_help, opts.help);                        // --help отработан верно

    mu_assert("Can't allocate memory", opts.files != NULL);  // Проверка что память выделена
    mu_assert("Сount out of range",
              opts.file_count > 0 &&
                  opts.file_count <= argc);  // Проверка что количество файлов в правильном диапазоне
    mu_assert_inteq(expected_file_count,
                    opts.file_count);  // Проверка что количество файлов считает правильно
    for (int i = 0; i < expected_file_count; i++) {
        mu_assert_streq(expected_argv[i], opts.files[i]);  // Проверка что имена файлов совпадают
    }

    OptionsFree(&opts);
    return 0;
}

// 7. stdin среди файлов
static int test_files7_and_options(void) {
    struct Options opts = {0};

    char *argv[] = {"file_name", "-n", "a.txt", "-", "b.txt", NULL};
    int argc = sizeof(argv) / sizeof(argv[0]) - 1;  // NULL в argc не входит

    bool expected_number_lines = true;      // -n
    bool expected_number_nonblank = false;  // -b
    bool expected_squeeze_blank = false;    // -s
    bool expected_show_ends = false;        // -E
    bool expected_show_tabs = false;        // -T
    bool expected_help = false;             // --help

    char *expected_argv[] = {"a.txt", "-", "b.txt"};
    int expected_file_count = sizeof(expected_argv) / sizeof(expected_argv[0]);

    int res = ParseArgs(argc, argv, &opts);
    int res_expected = 0;

    mu_assert_inteq(res_expected, res);  // Проверка кода возврата ParseArgs
    mu_assert_inteq(expected_number_lines, opts.number_lines);        // -n отработан верно
    mu_assert_inteq(expected_number_nonblank, opts.number_nonblank);  // -b отработан верно
    mu_assert_inteq(expected_squeeze_blank, opts.squeeze_blank);      // -s отработан верно
    mu_assert_inteq(expected_show_ends, opts.show_ends);              // -E отработан верно
    mu_assert_inteq(expected_show_tabs, opts.show_tabs);              // -T отработан верно
    mu_assert_inteq(expected_help, opts.help);                        // --help отработан верно

    mu_assert("Can't allocate memory", opts.files != NULL);  // Проверка что память выделена
    mu_assert("Сount out of range",
              opts.file_count > 0 &&
                  opts.file_count <= argc);  // Проверка что количество файлов в правильном диапазоне
    mu_assert_inteq(expected_file_count,
                    opts.file_count);  // Проверка что количество файлов считает правильно
    for (int i = 0; i < expected_file_count; i++) {
        mu_assert_streq(expected_argv[i], opts.files[i]);  // Проверка что имена файлов совпадают
    }

    OptionsFree(&opts);
    return 0;
}

// 6. все флаги и файл
static int test_files6_and_options(void) {
    struct Options opts = {0};

    char *argv[] = {"file_name", "-nbsET", "a.txt", NULL};
    int argc = sizeof(argv) / sizeof(argv[0]) - 1;  // NULL в argc не входит

    bool expected_number_lines = true;     // -n
    bool expected_number_nonblank = true;  // -b
    bool expected_squeeze_blank = true;    // -s
    bool expected_show_ends = true;        // -E
    bool expected_show_tabs = true;        // -T
    bool expected_help = false;            // --help

    char *expected_argv[] = {"a.txt"};
    int expected_file_count = sizeof(expected_argv) / sizeof(expected_argv[0]);

    int res = ParseArgs(argc, argv, &opts);
    int res_expected = 0;

    mu_assert_inteq(res_expected, res);  // Проверка кода возврата ParseArgs
    mu_assert_inteq(expected_number_lines, opts.number_lines);        // -n отработан верно
    mu_assert_inteq(expected_number_nonblank, opts.number_nonblank);  // -b отработан верно
    mu_assert_inteq(expected_squeeze_blank, opts.squeeze_blank);      // -s отработан верно
    mu_assert_inteq(expected_show_ends, opts.show_ends);              // -E отработан верно
    mu_assert_inteq(expected_show_tabs, opts.show_tabs);              // -T отработан верно
    mu_assert_inteq(expected_help, opts.help);                        // --help отработан верно

    mu_assert("Can't allocate memory", opts.files != NULL);  // Проверка что память выделена
    mu_assert("Сount out of range",
              opts.file_count > 0 &&
                  opts.file_count <= argc);  // Проверка что количество файлов в правильном диапазоне
    mu_assert_inteq(expected_file_count,
                    opts.file_count);  // Проверка что количество файлов считает правильно
    for (int i = 0; i < expected_file_count; i++) {
        mu_assert_streq(expected_argv[i], opts.files[i]);  // Проверка что имена файлов совпадают
    }

    OptionsFree(&opts);
    return 0;
}

// 5. склеенные флаги перед файлами
static int test_files5_and_options(void) {
    struct Options opts = {0};

    char *argv[] = {"file_name", "-nE", "a.txt", "b.txt", NULL};
    int argc = sizeof(argv) / sizeof(argv[0]) - 1;  // NULL в argc не входит

    bool expected_number_lines = true;      // -n
    bool expected_number_nonblank = false;  // -b
    bool expected_squeeze_blank = false;    // -s
    bool expected_show_ends = true;         // -E
    bool expected_show_tabs = false;        // -T
    bool expected_help = false;             // --help

    char *expected_argv[] = {"a.txt", "b.txt"};
    int expected_file_count = sizeof(expected_argv) / sizeof(expected_argv[0]);

    int res = ParseArgs(argc, argv, &opts);
    int res_expected = 0;

    mu_assert_inteq(res_expected, res);  // Проверка кода возврата ParseArgs
    mu_assert_inteq(expected_number_lines, opts.number_lines);        // -n отработан верно
    mu_assert_inteq(expected_number_nonblank, opts.number_nonblank);  // -b отработан верно
    mu_assert_inteq(expected_squeeze_blank, opts.squeeze_blank);      // -s отработан верно
    mu_assert_inteq(expected_show_ends, opts.show_ends);              // -E отработан верно
    mu_assert_inteq(expected_show_tabs, opts.show_tabs);              // -T отработан верно
    mu_assert_inteq(expected_help, opts.help);                        // --help отработан верно

    mu_assert("Can't allocate memory", opts.files != NULL);  // Проверка что память выделена
    mu_assert("Сount out of range",
              opts.file_count > 0 &&
                  opts.file_count <= argc);  // Проверка что количество файлов в правильном диапазоне
    mu_assert_inteq(expected_file_count,
                    opts.file_count);  // Проверка что количество файлов считает правильно
    for (int i = 0; i < expected_file_count; i++) {
        mu_assert_streq(expected_argv[i], opts.files[i]);  // Проверка что имена файлов совпадают
    }

    OptionsFree(&opts);
    return 0;
}

// 4. флаги и файлы вперемешку
static int test_files4_and_options(void) {
    struct Options opts = {0};

    char *argv[] = {"file_name", "-n", "a.txt", "-E", "b.txt", "-T", NULL};
    int argc = sizeof(argv) / sizeof(argv[0]) - 1;  // NULL в argc не входит

    bool expected_number_lines = true;      // -n
    bool expected_number_nonblank = false;  // -b
    bool expected_squeeze_blank = false;    // -s
    bool expected_show_ends = true;         // -E
    bool expected_show_tabs = true;         // -T
    bool expected_help = false;             // --help

    char *expected_argv[] = {"a.txt", "b.txt"};
    int expected_file_count = sizeof(expected_argv) / sizeof(expected_argv[0]);

    int res = ParseArgs(argc, argv, &opts);
    int res_expected = 0;

    mu_assert_inteq(res_expected, res);  // Проверка кода возврата ParseArgs
    mu_assert_inteq(expected_number_lines, opts.number_lines);        // -n отработан верно
    mu_assert_inteq(expected_number_nonblank, opts.number_nonblank);  // -b отработан верно
    mu_assert_inteq(expected_squeeze_blank, opts.squeeze_blank);      // -s отработан верно
    mu_assert_inteq(expected_show_ends, opts.show_ends);              // -E отработан верно
    mu_assert_inteq(expected_show_tabs, opts.show_tabs);              // -T отработан верно
    mu_assert_inteq(expected_help, opts.help);                        // --help отработан верно

    mu_assert("Can't allocate memory", opts.files != NULL);  // Проверка что память выделена
    mu_assert("Сount out of range",
              opts.file_count > 0 &&
                  opts.file_count <= argc);  // Проверка что количество файлов в правильном диапазоне
    mu_assert_inteq(expected_file_count,
                    opts.file_count);  // Проверка что количество файлов считает правильно
    for (int i = 0; i < expected_file_count; i++) {
        mu_assert_streq(expected_argv[i], opts.files[i]);  // Проверка что имена файлов совпадают
    }

    OptionsFree(&opts);
    return 0;
}

// 3. флаг между файлами
static int test_files3_and_options(void) {
    struct Options opts = {0};

    char *argv[] = {"file_name", "a.txt", "-n", "b.txt", NULL};
    int argc = sizeof(argv) / sizeof(argv[0]) - 1;  // NULL в argc не входит

    bool expected_number_lines = true;      // -n
    bool expected_number_nonblank = false;  // -b
    bool expected_squeeze_blank = false;    // -s
    bool expected_show_ends = false;        // -E
    bool expected_show_tabs = false;        // -T
    bool expected_help = false;             // --help

    char *expected_argv[] = {"a.txt", "b.txt"};
    int expected_file_count = sizeof(expected_argv) / sizeof(expected_argv[0]);

    int res = ParseArgs(argc, argv, &opts);
    int res_expected = 0;

    mu_assert_inteq(res_expected, res);  // Проверка кода возврата ParseArgs
    mu_assert_inteq(expected_number_lines, opts.number_lines);        // -n отработан верно
    mu_assert_inteq(expected_number_nonblank, opts.number_nonblank);  // -b отработан верно
    mu_assert_inteq(expected_squeeze_blank, opts.squeeze_blank);      // -s отработан верно
    mu_assert_inteq(expected_show_ends, opts.show_ends);              // -E отработан верно
    mu_assert_inteq(expected_show_tabs, opts.show_tabs);              // -T отработан верно
    mu_assert_inteq(expected_help, opts.help);                        // --help отработан верно

    mu_assert("Can't allocate memory", opts.files != NULL);  // Проверка что память выделена
    mu_assert("Сount out of range",
              opts.file_count > 0 &&
                  opts.file_count <= argc);  // Проверка что количество файлов в правильном диапазоне
    mu_assert_inteq(expected_file_count,
                    opts.file_count);  // Проверка что количество файлов считает правильно
    for (int i = 0; i < expected_file_count; i++) {
        mu_assert_streq(expected_argv[i], opts.files[i]);  // Проверка что имена файлов совпадают
    }

    OptionsFree(&opts);
    return 0;
}

// 2. флаг после файла
static int test_files2_and_options(void) {
    struct Options opts = {0};

    char *argv[] = {"file_name", "a.txt", "-n", NULL};
    int argc = sizeof(argv) / sizeof(argv[0]) - 1;  // NULL в argc не входит

    bool expected_number_lines = true;      // -n
    bool expected_number_nonblank = false;  // -b
    bool expected_squeeze_blank = false;    // -s
    bool expected_show_ends = false;        // -E
    bool expected_show_tabs = false;        // -T
    bool expected_help = false;             // --help

    char *expected_argv[] = {"a.txt"};
    int expected_file_count = sizeof(expected_argv) / sizeof(expected_argv[0]);

    int res = ParseArgs(argc, argv, &opts);
    int res_expected = 0;

    mu_assert_inteq(res_expected, res);  // Проверка кода возврата ParseArgs
    mu_assert_inteq(expected_number_lines, opts.number_lines);        // -n отработан верно
    mu_assert_inteq(expected_number_nonblank, opts.number_nonblank);  // -b отработан верно
    mu_assert_inteq(expected_squeeze_blank, opts.squeeze_blank);      // -s отработан верно
    mu_assert_inteq(expected_show_ends, opts.show_ends);              // -E отработан верно
    mu_assert_inteq(expected_show_tabs, opts.show_tabs);              // -T отработан верно
    mu_assert_inteq(expected_help, opts.help);                        // --help отработан верно

    mu_assert("Can't allocate memory", opts.files != NULL);  // Проверка что память выделена
    mu_assert("Сount out of range",
              opts.file_count > 0 &&
                  opts.file_count <= argc);  // Проверка что количество файлов в правильном диапазоне
    mu_assert_inteq(expected_file_count,
                    opts.file_count);  // Проверка что количество файлов считает правильно
    for (int i = 0; i < expected_file_count; i++) {
        mu_assert_streq(expected_argv[i], opts.files[i]);  // Проверка что имена файлов совпадают
    }

    OptionsFree(&opts);
    return 0;
}

// 1. флаг перед файлом
static int test_files1_and_options(void) {
    struct Options opts = {0};

    char *argv[] = {"file_name", "-n", "a.txt", NULL};
    int argc = sizeof(argv) / sizeof(argv[0]) - 1;  // NULL в argc не входит

    bool expected_number_lines = true;      // -n
    bool expected_number_nonblank = false;  // -b
    bool expected_squeeze_blank = false;    // -s
    bool expected_show_ends = false;        // -E
    bool expected_show_tabs = false;        // -T
    bool expected_help = false;             // --help

    char *expected_argv[] = {"a.txt"};
    int expected_file_count = sizeof(expected_argv) / sizeof(expected_argv[0]);

    int res = ParseArgs(argc, argv, &opts);
    int res_expected = 0;

    mu_assert_inteq(res_expected, res);  // Проверка кода возврата ParseArgs
    mu_assert_inteq(expected_number_lines, opts.number_lines);        // -n отработан верно
    mu_assert_inteq(expected_number_nonblank, opts.number_nonblank);  // -b отработан верно
    mu_assert_inteq(expected_squeeze_blank, opts.squeeze_blank);      // -s отработан верно
    mu_assert_inteq(expected_show_ends, opts.show_ends);              // -E отработан верно
    mu_assert_inteq(expected_show_tabs, opts.show_tabs);              // -T отработан верно
    mu_assert_inteq(expected_help, opts.help);                        // --help отработан верно

    mu_assert("Can't allocate memory", opts.files != NULL);  // Проверка что память выделена
    mu_assert("Сount out of range",
              opts.file_count > 0 &&
                  opts.file_count <= argc);  // Проверка что количество файлов в правильном диапазоне
    mu_assert_inteq(expected_file_count,
                    opts.file_count);  // Проверка что количество файлов считает правильно
    for (int i = 0; i < expected_file_count; i++) {
        mu_assert_streq(expected_argv[i], opts.files[i]);  // Проверка что имена файлов совпадают
    }

    OptionsFree(&opts);
    return 0;
}

// Тесты для опций смешанные

// 9. неизвестный флаг в конце склейки
static int test_all9_options(void) {
    struct Options opts = {0};

    char *argv[] = {"file_name", "-nbsEt", NULL};
    int argc = sizeof(argv) / sizeof(argv[0]) - 1;  // NULL в argc не входит

    bool expected_number_lines = true;     // -n
    bool expected_number_nonblank = true;  // -b
    bool expected_squeeze_blank = true;    // -s
    bool expected_show_ends = true;        // -E
    bool expected_show_tabs = false;       // -T
    bool expected_help = false;            // --help

    int res = ParseArgs(argc, argv, &opts);
    int res_expected = 2;  // ParseArgs упала при разборе

    mu_assert_inteq(res_expected, res);  // Проверка что ParseArgs не упал
    mu_assert_inteq(expected_number_lines, opts.number_lines);        // -n отработан верно
    mu_assert_inteq(expected_number_nonblank, opts.number_nonblank);  // -b отработан верно
    mu_assert_inteq(expected_squeeze_blank, opts.squeeze_blank);      // -s отработан верно
    mu_assert_inteq(expected_show_ends, opts.show_ends);              // -E отработан верно
    mu_assert_inteq(expected_show_tabs, opts.show_tabs);              // -T отработан верно
    mu_assert_inteq(expected_help, opts.help);                        // --help оработан верно

    OptionsFree(&opts);
    return 0;
}

// 8. все флаги в неверном регистре
static int test_all8_options(void) {
    struct Options opts = {0};

    char *argv[] = {"file_name", "-N", "-B", "-S", "-e", "-t", "--help", NULL};
    int argc = sizeof(argv) / sizeof(argv[0]) - 1;  // NULL в argc не входит

    bool expected_number_lines = false;     // -n
    bool expected_number_nonblank = false;  // -b
    bool expected_squeeze_blank = false;    // -s
    bool expected_show_ends = false;        // -E
    bool expected_show_tabs = false;        // -T
    bool expected_help = false;             // --help

    int res = ParseArgs(argc, argv, &opts);
    int res_expected = 2;  // ParseArgs упала при разборе

    mu_assert_inteq(res_expected, res);  // Проверка что ParseArgs не упал
    mu_assert_inteq(expected_number_lines, opts.number_lines);        // -n отработан верно
    mu_assert_inteq(expected_number_nonblank, opts.number_nonblank);  // -b отработан верно
    mu_assert_inteq(expected_squeeze_blank, opts.squeeze_blank);      // -s отработан верно
    mu_assert_inteq(expected_show_ends, opts.show_ends);              // -E отработан верно
    mu_assert_inteq(expected_show_tabs, opts.show_tabs);              // -T отработан верно
    mu_assert_inteq(expected_help, opts.help);                        // --help оработан верно

    OptionsFree(&opts);
    return 0;
}

// 7. неизвестный флаг первым, верный после него не читается
static int test_all7_options(void) {
    struct Options opts = {0};

    char *argv[] = {"file_name", "-t", "-n", NULL};
    int argc = sizeof(argv) / sizeof(argv[0]) - 1;  // NULL в argc не входит

    bool expected_number_lines = false;     // -n
    bool expected_number_nonblank = false;  // -b
    bool expected_squeeze_blank = false;    // -s
    bool expected_show_ends = false;        // -E
    bool expected_show_tabs = false;        // -T
    bool expected_help = false;             // --help

    int res = ParseArgs(argc, argv, &opts);
    int res_expected = 2;  // ParseArgs упала при разборе

    mu_assert_inteq(res_expected, res);  // Проверка что ParseArgs не упал
    mu_assert_inteq(expected_number_lines, opts.number_lines);        // -n отработан верно
    mu_assert_inteq(expected_number_nonblank, opts.number_nonblank);  // -b отработан верно
    mu_assert_inteq(expected_squeeze_blank, opts.squeeze_blank);      // -s отработан верно
    mu_assert_inteq(expected_show_ends, opts.show_ends);              // -E отработан верно
    mu_assert_inteq(expected_show_tabs, opts.show_tabs);              // -T отработан верно
    mu_assert_inteq(expected_help, opts.help);                        // --help оработан верно

    OptionsFree(&opts);
    return 0;
}

// 6. разбор прерывается на неизвестном флаге, остальное не читается
static int test_all6_options(void) {
    struct Options opts = {0};

    char *argv[] = {"file_name", "-n", "-t", "-n", NULL};
    int argc = sizeof(argv) / sizeof(argv[0]) - 1;  // NULL в argc не входит

    bool expected_number_lines = true;      // -n
    bool expected_number_nonblank = false;  // -b
    bool expected_squeeze_blank = false;    // -s
    bool expected_show_ends = false;        // -E
    bool expected_show_tabs = false;        // -T
    bool expected_help = false;             // --help

    int res = ParseArgs(argc, argv, &opts);
    int res_expected = 2;  // ParseArgs упала при разборе

    mu_assert_inteq(res_expected, res);  // Проверка что ParseArgs не упал
    mu_assert_inteq(expected_number_lines, opts.number_lines);        // -n отработан верно
    mu_assert_inteq(expected_number_nonblank, opts.number_nonblank);  // -b отработан верно
    mu_assert_inteq(expected_squeeze_blank, opts.squeeze_blank);      // -s отработан верно
    mu_assert_inteq(expected_show_ends, opts.show_ends);              // -E отработан верно
    mu_assert_inteq(expected_show_tabs, opts.show_tabs);              // -T отработан верно
    mu_assert_inteq(expected_help, opts.help);                        // --help оработан верно

    OptionsFree(&opts);
    return 0;
}

// 5. верный флаг, затем неизвестный
static int test_all5_options(void) {
    struct Options opts = {0};

    char *argv[] = {"file_name", "-n", "-t", NULL};
    int argc = sizeof(argv) / sizeof(argv[0]) - 1;  // NULL в argc не входит

    bool expected_number_lines = true;      // -n
    bool expected_number_nonblank = false;  // -b
    bool expected_squeeze_blank = false;    // -s
    bool expected_show_ends = false;        // -E
    bool expected_show_tabs = false;        // -T
    bool expected_help = false;             // --help

    int res = ParseArgs(argc, argv, &opts);
    int res_expected = 2;  // ParseArgs упала при разборе

    mu_assert_inteq(res_expected, res);  // Проверка что ParseArgs не упал
    mu_assert_inteq(expected_number_lines, opts.number_lines);        // -n отработан верно
    mu_assert_inteq(expected_number_nonblank, opts.number_nonblank);  // -b отработан верно
    mu_assert_inteq(expected_squeeze_blank, opts.squeeze_blank);      // -s отработан верно
    mu_assert_inteq(expected_show_ends, opts.show_ends);              // -E отработан верно
    mu_assert_inteq(expected_show_tabs, opts.show_tabs);              // -T отработан верно
    mu_assert_inteq(expected_help, opts.help);                        // --help оработан верно

    OptionsFree(&opts);
    return 0;
}

// 4. неизвестный флаг (-t вместо -T)
static int test_all4_options(void) {
    struct Options opts = {0};

    char *argv[] = {"file_name", "-t", NULL};
    int argc = sizeof(argv) / sizeof(argv[0]) - 1;  // NULL в argc не входит

    bool expected_number_lines = false;     // -n
    bool expected_number_nonblank = false;  // -b
    bool expected_squeeze_blank = false;    // -s
    bool expected_show_ends = false;        // -E
    bool expected_show_tabs = false;        // -T
    bool expected_help = false;             // --help

    int res = ParseArgs(argc, argv, &opts);
    int res_expected = 2;  // ParseArgs упала при разборе

    mu_assert_inteq(res_expected, res);  // Проверка что ParseArgs не упал
    mu_assert_inteq(expected_number_lines, opts.number_lines);        // -n отработан верно
    mu_assert_inteq(expected_number_nonblank, opts.number_nonblank);  // -b отработан верно
    mu_assert_inteq(expected_squeeze_blank, opts.squeeze_blank);      // -s отработан верно
    mu_assert_inteq(expected_show_ends, opts.show_ends);              // -E отработан верно
    mu_assert_inteq(expected_show_tabs, opts.show_tabs);              // -T отработан верно
    mu_assert_inteq(expected_help, opts.help);                        // --help оработан верно

    OptionsFree(&opts);
    return 0;
}

// 3. флаги склеены в две группы
static int test_all3_options(void) {
    struct Options opts = {0};

    char *argv[] = {"file_name", "-nb", "-sET", NULL};
    int argc = sizeof(argv) / sizeof(argv[0]) - 1;  // NULL в argc не входит

    bool expected_number_lines = true;     // -n
    bool expected_number_nonblank = true;  // -b
    bool expected_squeeze_blank = true;    // -s
    bool expected_show_ends = true;        // -E
    bool expected_show_tabs = true;        // -T
    bool expected_help = false;            // --help

    int res = ParseArgs(argc, argv, &opts);
    int res_expected = 0;

    mu_assert_inteq(res_expected, res);  // Проверка что ParseArgs не упал
    mu_assert_inteq(expected_number_lines, opts.number_lines);        // -n отработан верно
    mu_assert_inteq(expected_number_nonblank, opts.number_nonblank);  // -b отработан верно
    mu_assert_inteq(expected_squeeze_blank, opts.squeeze_blank);      // -s отработан верно
    mu_assert_inteq(expected_show_ends, opts.show_ends);              // -E отработан верно
    mu_assert_inteq(expected_show_tabs, opts.show_tabs);              // -T отработан верно
    mu_assert_inteq(expected_help, opts.help);                        // --help оработан верно

    OptionsFree(&opts);
    return 0;
}

// 2. все флаги склеены в один аргумент
static int test_all2_options(void) {
    struct Options opts = {0};

    char *argv[] = {"file_name", "-nbsET", NULL};
    int argc = sizeof(argv) / sizeof(argv[0]) - 1;  // NULL в argc не входит

    bool expected_number_lines = true;     // -n
    bool expected_number_nonblank = true;  // -b
    bool expected_squeeze_blank = true;    // -s
    bool expected_show_ends = true;        // -E
    bool expected_show_tabs = true;        // -T
    bool expected_help = false;            // --help

    int res = ParseArgs(argc, argv, &opts);
    int res_expected = 0;

    mu_assert_inteq(res_expected, res);  // Проверка что ParseArgs не упал
    mu_assert_inteq(expected_number_lines, opts.number_lines);        // -n отработан верно
    mu_assert_inteq(expected_number_nonblank, opts.number_nonblank);  // -b отработан верно
    mu_assert_inteq(expected_squeeze_blank, opts.squeeze_blank);      // -s отработан верно
    mu_assert_inteq(expected_show_ends, opts.show_ends);              // -E отработан верно
    mu_assert_inteq(expected_show_tabs, opts.show_tabs);              // -T отработан верно
    mu_assert_inteq(expected_help, opts.help);                        // --help оработан верно

    OptionsFree(&opts);
    return 0;
}

// Тесты для опций позитивные

// 1. все флаги по отдельности
static int test_all1_options(void) {
    struct Options opts = {0};

    char *argv[] = {"file_name", "-n", "-b", "-s", "-E", "-T", "--help", NULL};
    int argc = sizeof(argv) / sizeof(argv[0]) - 1;  // NULL в argc не входит

    bool expected_number_lines = true;     // -n
    bool expected_number_nonblank = true;  // -b
    bool expected_squeeze_blank = true;    // -s
    bool expected_show_ends = true;        // -E
    bool expected_show_tabs = true;        // -T
    bool expected_help = true;             // --help

    int res = ParseArgs(argc, argv, &opts);
    int res_expected = HELP_REQUESTED;  // --help стоит последним, флаги уже разобраны

    mu_assert_inteq(res_expected, res);  // Проверка что ParseArgs не упал
    mu_assert_inteq(expected_number_lines, opts.number_lines);        // -n отработан верно
    mu_assert_inteq(expected_number_nonblank, opts.number_nonblank);  // -b отработан верно
    mu_assert_inteq(expected_squeeze_blank, opts.squeeze_blank);      // -s отработан верно
    mu_assert_inteq(expected_show_ends, opts.show_ends);              // -E отработан верно
    mu_assert_inteq(expected_show_tabs, opts.show_tabs);              // -T отработан верно
    mu_assert_inteq(expected_help, opts.help);                        // --help оработан верно

    OptionsFree(&opts);
    return 0;
}

// Тесты для имен файлов

static int test_file_names_dushes_options(void) {
    struct Options opts = {0};

    char *argv[] = {"file_name", "filename1", "-", "filename2", NULL};
    int argc = sizeof(argv) / sizeof(argv[0]) - 1;  // NULL в argc не входит

    char *expected_argv[] = {"filename1", "-", "filename2"};
    int expected_file_count = sizeof(expected_argv) / sizeof(expected_argv[0]);

    int res = ParseArgs(argc, argv, &opts);
    int res_expected = 0;

    mu_assert_inteq(res_expected, res);  // Проверка что ParseArgs не упал
    mu_assert("Can't allocate memory", opts.files != NULL);  // Проверка что память выделена
    mu_assert("Сount out of range",
              opts.file_count > 0 &&
                  opts.file_count <= argc);  // Проверка что колиество файлов в правильном диапазоне
    mu_assert_inteq(expected_file_count,
                    opts.file_count);  // Проверка что количество файлов считает правильно
    for (int i = 0; i < expected_file_count; i++) {
        mu_assert_streq(expected_argv[i], opts.files[i]);  //  Проверка что имена файлов совпадают
    }

    OptionsFree(&opts);
    return 0;
}

static int test_dushes_file_names_options(void) {
    struct Options opts = {0};

    char *argv[] = {"file_name", "-", "filename1", "-", "filename2", "-", NULL};
    int argc = sizeof(argv) / sizeof(argv[0]) - 1;  // NULL в argc не входит

    char *expected_argv[] = {"-", "filename1", "-", "filename2", "-"};
    int expected_file_count = sizeof(expected_argv) / sizeof(expected_argv[0]);

    int res = ParseArgs(argc, argv, &opts);
    int res_expected = 0;

    mu_assert_inteq(res_expected, res);  // Проверка что ParseArgs не упал
    mu_assert("Can't allocate memory", opts.files != NULL);  // Проверка что память выделена
    mu_assert("Сount out of range",
              opts.file_count > 0 &&
                  opts.file_count <= argc);  // Проверка что колиество файлов в правильном диапазоне
    mu_assert_inteq(expected_file_count,
                    opts.file_count);  // Проверка что количество файлов считает правильно
    for (int i = 0; i < expected_file_count; i++) {
        mu_assert_streq(expected_argv[i], opts.files[i]);  //  Проверка что имена файлов совпадают
    }

    OptionsFree(&opts);
    return 0;
}

static int test_two_file_names_options(void) {
    struct Options opts = {0};

    char *argv[] = {"file_name", "filename1", "filename2", NULL};
    int argc = sizeof(argv) / sizeof(argv[0]) - 1;  // NULL в argc не входит

    char *expected_argv[] = {"filename1", "filename2"};
    int expected_file_count = sizeof(expected_argv) / sizeof(expected_argv[0]);

    int res = ParseArgs(argc, argv, &opts);
    int res_expected = 0;

    mu_assert_inteq(res_expected, res);  // Проверка что ParseArgs не упал
    mu_assert("Can't allocate memory", opts.files != NULL);  // Проверка что память выделена
    mu_assert("Сount out of range",
              opts.file_count > 0 &&
                  opts.file_count <= argc);  // Проверка что колиество файлов в правильном диапазоне
    mu_assert_inteq(expected_file_count,
                    opts.file_count);  // Проверка что количество файлов считает правильно
    for (int i = 0; i < expected_file_count; i++) {
        mu_assert_streq(expected_argv[i], opts.files[i]);  //  Проверка что имена файлов совпадают
    }

    OptionsFree(&opts);
    return 0;
}

static int test_one_file_name_options(void) {
    struct Options opts = {0};

    char *argv[] = {"file_name", "filename1", NULL};
    int argc = sizeof(argv) / sizeof(argv[0]) - 1;  // NULL в argc не входит

    char *expected_argv[] = {"filename1"};
    int expected_file_count = sizeof(expected_argv) / sizeof(expected_argv[0]);

    int res = ParseArgs(argc, argv, &opts);
    int res_expected = 0;

    mu_assert_inteq(res_expected, res);  // Проверка что ParseArgs не упал
    mu_assert("Can't allocate memory", opts.files != NULL);  // Проверка что память выделена
    mu_assert("Сount out of range",
              opts.file_count > 0 &&
                  opts.file_count <= argc);  // Проверка что колиество файлов в правильном диапазоне
    mu_assert_inteq(expected_file_count,
                    opts.file_count);  // Проверка что количество файлов считает правильно
    for (int i = 0; i < expected_file_count; i++) {
        mu_assert_streq(expected_argv[i], opts.files[i]);  //  Проверка что имена файлов совпадают
    }

    OptionsFree(&opts);
    return 0;
}

static int test_two_dash_options(void) {
    struct Options opts = {0};

    char *argv[] = {"file_name", "-", "-", NULL};
    int argc = sizeof(argv) / sizeof(argv[0]) - 1;  // NULL в argc не входит

    char *expected_argv[] = {"-", "-"};
    int expected_file_count = sizeof(expected_argv) / sizeof(expected_argv[0]);

    int res = ParseArgs(argc, argv, &opts);
    int res_expected = 0;

    mu_assert_inteq(res_expected, res);  // Проверка что ParseArgs не упал
    mu_assert("Can't allocate memory", opts.files != NULL);  // Проверка что память выделена
    mu_assert("Сount out of range",
              opts.file_count > 0 &&
                  opts.file_count <= argc);  // Проверка что колиество файлов в правильном диапазоне
    mu_assert_inteq(expected_file_count,
                    opts.file_count);  // Проверка что количество файлов считает правильно
    for (int i = 0; i < expected_file_count; i++) {
        mu_assert_streq(expected_argv[i], opts.files[i]);  //  Проверка что имена файлов совпадают
    }

    OptionsFree(&opts);
    return 0;
}

static int test_dash_options(void) {
    struct Options opts = {0};

    char *argv[] = {"file_name", "-", NULL};
    int argc = sizeof(argv) / sizeof(argv[0]) - 1;  // NULL в argc не входит

    char *expected_argv[] = {"-"};
    int expected_file_count = sizeof(expected_argv) / sizeof(expected_argv[0]);

    int res = ParseArgs(argc, argv, &opts);
    int res_expected = 0;

    mu_assert_inteq(res_expected, res);  // Проверка что ParseArgs не упал
    mu_assert("Can't allocate memory", opts.files != NULL);  // Проверка что память выделена
    mu_assert("Сount out of range",
              opts.file_count > 0 &&
                  opts.file_count <= argc);  // Проверка что колиество файлов в правильном диапазоне
    mu_assert_inteq(expected_file_count,
                    opts.file_count);  // Проверка что количество файлов считает правильно
    for (int i = 0; i < expected_file_count; i++) {
        mu_assert_streq(expected_argv[i], opts.files[i]);  //  Проверка что имена файлов совпадают
    }

    OptionsFree(&opts);
    return 0;
}

static int test_null_options(void) {
    struct Options opts = {0};

    char *argv[] = {"file_name", NULL};
    int argc = sizeof(argv) / sizeof(argv[0]) - 1;  // NULL в argc не входит

    char *expected_argv[] = {"-"};
    int expected_file_count = sizeof(expected_argv) / sizeof(expected_argv[0]);

    int res = ParseArgs(argc, argv, &opts);
    int res_expected = 0;

    mu_assert_inteq(res_expected, res);  // Проверка что ParseArgs не упал
    mu_assert("Can't allocate memory", opts.files != NULL);  // Проверка что память выделена
    mu_assert("Сount out of range",
              opts.file_count > 0 &&
                  opts.file_count <= argc);  // Проверка что колиество файлов в правильном диапазоне
    mu_assert_inteq(expected_file_count,
                    opts.file_count);  // Проверка что количество файлов считает правильно
    for (int i = 0; i < expected_file_count; i++) {
        mu_assert_streq(expected_argv[i], opts.files[i]);  //  Проверка что имена файлов совпадают
    }

    OptionsFree(&opts);
    return 0;
}

// Тесты для PrintHelp
//
// PrintHelp принимает FILE *out, поэтому stdout подменять не нужно: справка
// печатается в память через fmemopen и читается как обычная строка.
// Точный текст намеренно не сверяется — он меняется при правке формулировок,
// и тест ломался бы на каждой запятой. Проверяются свойства, которые обязаны
// держаться: куда пишем, что перечислено и чего там быть не должно.

// Печатает справку в buf. Возвращает false, если поток создать не удалось.
static bool PrintHelpToBuf(char *buf, size_t size) {
    buf[0] = '\0';

    FILE *out = fmemopen(buf, size, "w");
    if (out == NULL) return false;

    PrintHelp(out);

    return fclose(out) == 0;  // fclose сбрасывает буфер и дописывает '\0'
}

// 6. Повторный вызов печатает то же самое: у PrintHelp нет внутреннего состояния
static int test_help_is_repeatable(void) {
    char first[1024];
    char second[1024];

    mu_assert("fmemopen failed", PrintHelpToBuf(first, sizeof(first)));
    mu_assert("fmemopen failed", PrintHelpToBuf(second, sizeof(second)));

    mu_assert_streq(first, second);  // Проверка что текст не зависит от вызова

    return 0;
}

// 5. Текст заканчивается переводом строки, иначе приглашение оболочки прилипнет к справке
static int test_help_ends_with_newline(void) {
    char buf[1024];

    mu_assert("fmemopen failed", PrintHelpToBuf(buf, sizeof(buf)));

    size_t len = strlen(buf);

    mu_assert("help must not be empty", len > 0);  // Проверка что текст есть
    mu_assert("help must end with a newline", buf[len - 1] == '\n');  // Проверка перевода строки

    return 0;
}

// 4. Нереализованных флагов в справке нет (см. «Границы» в плане)
static int test_help_has_no_unimplemented_flags(void) {
    char buf[1024];

    mu_assert("fmemopen failed", PrintHelpToBuf(buf, sizeof(buf)));

    // Два пробела перед дефисом — так флаг выглядит в списке, обычное "-n"
    // встречается и в тексте описаний ("overrides -n")
    mu_assert("-A is not implemented", strstr(buf, "  -A") == NULL);  // Проверка что -A не обещан
    mu_assert("-v is not implemented", strstr(buf, "  -v") == NULL);  // Проверка что -v не обещан
    mu_assert("-e is not implemented", strstr(buf, "  -e") == NULL);  // Проверка что -e не обещан
    mu_assert("-t is not implemented", strstr(buf, "  -t") == NULL);  // Проверка что -t не обещан
    mu_assert("-u is not implemented", strstr(buf, "  -u") == NULL);  // Проверка что -u не обещан

    return 0;
}

// 3. Перечислены все реализованные флаги: справка не должна отставать от кода
static int test_help_lists_all_flags(void) {
    char buf[1024];

    mu_assert("fmemopen failed", PrintHelpToBuf(buf, sizeof(buf)));

    mu_assert("-b missing in help", strstr(buf, "  -b") != NULL);  // Проверка что -b описан
    mu_assert("-E missing in help", strstr(buf, "  -E") != NULL);  // Проверка что -E описан
    mu_assert("-n missing in help", strstr(buf, "  -n") != NULL);  // Проверка что -n описан
    mu_assert("-s missing in help", strstr(buf, "  -s") != NULL);  // Проверка что -s описан
    mu_assert("-T missing in help", strstr(buf, "  -T") != NULL);  // Проверка что -T описан
    mu_assert("--help missing in help", strstr(buf, "--help") != NULL);  // Проверка что --help описан

    return 0;
}

// 2. Шапка: строка вызова и объяснение, что без файлов читается stdin
static int test_help_header(void) {
    char buf[1024];

    mu_assert("fmemopen failed", PrintHelpToBuf(buf, sizeof(buf)));

    mu_assert("help must start with a usage line",
              strncmp(buf, "Usage: cat", 10) == 0);  // Проверка первой строки
    mu_assert("help must explain reading from stdin",
              strstr(buf, "standard input") != NULL);  // Проверка упоминания stdin

    return 0;
}

// 1. Справка пишется в переданный поток с текущей позиции и ничего не затирает
static int test_help_writes_to_stream(void) {
    char buf[1024];
    buf[0] = '\0';

    FILE *out = fmemopen(buf, sizeof(buf), "w");
    mu_assert("fmemopen failed", out != NULL);

    fputs(MARK, out);  // в потоке уже что-то есть
    PrintHelp(out);

    mu_assert("fclose failed", fclose(out) == 0);

    mu_assert("PrintHelp must not rewind or overwrite the stream",
              strncmp(buf, MARK, strlen(MARK)) == 0);  // Проверка что метка уцелела
    mu_assert("help must be written after it",
              strstr(buf + strlen(MARK), "Usage: cat") != NULL);  // Проверка что справка следом

    return 0;
}
