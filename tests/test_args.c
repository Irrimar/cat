#include "minunit.h"
#include "../src/args.h"

static int test_null_options(void);
static int test_dash_options(void);
static int test_two_dash_options(void);
static int test_one_file_name_options(void);
static int test_two_file_names_options(void);
static int test_dushes_file_names_options(void);
static int test_file_names_dushes_options(void);

int main(void) {
    mu_run_test(test_null_options);
    mu_run_test(test_dash_options);
    mu_run_test(test_two_dash_options);
    mu_run_test(test_one_file_name_options);
    mu_run_test(test_two_file_names_options);
    mu_run_test(test_dushes_file_names_options);
    mu_run_test(test_file_names_dushes_options);

    // mu_run_test(test_placeholder);
    mu_report();
    return mu_tests_failed != 0;
}

/* заглушка, пока args.c пуст
static int test_placeholder(void) {
    mu_assert("placeholder", 1);
    return 0;
}*/

static int test_file_names_dushes_options(void) {
    struct Options opts = {0};

    char *argv[] = {"file_name", "filename1", "-", "filename2", NULL};
    int argc = sizeof(argv) / sizeof(argv[0]) - 1;  // NULL в argc не входит

    char *expected_argv[] = {"filename1", "-", "filename2",};
    int expected_file_count = sizeof(expected_argv) / sizeof(expected_argv[0]);
    
    ParseArgs(argc, argv, &opts);
    mu_assert_inteq(expected_file_count, opts.file_count);
    for (int i = 0; i < expected_file_count; i++)
        mu_assert_streq(expected_argv[i], opts.files[i]);

    OptionsFree(&opts);

    return 0;
}

static int test_dushes_file_names_options(void) {
    struct Options opts = {0};

    char *argv[] = {"file_name", "-", "filename1", "-", "filename2", "-", NULL};
    int argc = sizeof(argv) / sizeof(argv[0]) - 1;  // NULL в argc не входит

    char *expected_argv[] = {"-", "filename1", "-", "filename2", "-"};
    int expected_file_count = sizeof(expected_argv) / sizeof(expected_argv[0]);
    
    ParseArgs(argc, argv, &opts);
    mu_assert_inteq(expected_file_count, opts.file_count);
    for (int i = 0; i < expected_file_count; i++)
        mu_assert_streq(expected_argv[i], opts.files[i]);

    OptionsFree(&opts);

    return 0;
}

static int test_two_file_names_options(void) {
    struct Options opts = {0};

    char *argv[] = {"file_name", "filename1", "filename2", NULL};
    int argc = sizeof(argv) / sizeof(argv[0]) - 1;  // NULL в argc не входит

    char *expected_argv[] = {"filename1", "filename2"};
    int expected_file_count = sizeof(expected_argv) / sizeof(expected_argv[0]);
    
    ParseArgs(argc, argv, &opts);
    mu_assert_inteq(expected_file_count, opts.file_count);
    for (int i = 0; i < expected_file_count; i++)
        mu_assert_streq(expected_argv[i], opts.files[i]);

    OptionsFree(&opts);

    return 0;
}

static int test_one_file_name_options(void) {
    struct Options opts = {0};

    char *argv[] = {"file_name", "filename1", NULL};
    int argc = sizeof(argv) / sizeof(argv[0]) - 1;  // NULL в argc не входит

    char *expected_argv[] = {"filename1"};
    int expected_file_count = sizeof(expected_argv) / sizeof(expected_argv[0]);
    
    ParseArgs(argc, argv, &opts);
    mu_assert_inteq(expected_file_count, opts.file_count);
    for (int i = 0; i < expected_file_count; i++)
        mu_assert_streq(expected_argv[i], opts.files[i]);

    OptionsFree(&opts);

    return 0;
}

static int test_two_dash_options(void) {
    struct Options opts = {0};

    char *argv[] = {"file_name", "-", "-", NULL};
    int argc = sizeof(argv) / sizeof(argv[0]) - 1;  // NULL в argc не входит

    char *expected_argv[] = {"-", "-"};
    int expected_file_count = sizeof(expected_argv) / sizeof(expected_argv[0]);
    
    ParseArgs(argc, argv, &opts);
    mu_assert_inteq(expected_file_count, opts.file_count);
    for (int i = 0; i < expected_file_count; i++)
        mu_assert_streq(expected_argv[i], opts.files[i]);

    OptionsFree(&opts);

    return 0;
}

static int test_dash_options(void) {
    struct Options opts = {0};

    char *argv[] = {"file_name", "-", NULL};
    int argc = sizeof(argv) / sizeof(argv[0]) - 1;  // NULL в argc не входит

    char *expected_argv[] = {"-"};
    int expected_file_count = sizeof(expected_argv) / sizeof(expected_argv[0]);
    
    ParseArgs(argc, argv, &opts);
    mu_assert_inteq(expected_file_count, opts.file_count);
    for (int i = 0; i < expected_file_count; i++)
        mu_assert_streq(expected_argv[i], opts.files[i]);

    OptionsFree(&opts);

    return 0;
}

static int test_null_options(void) {
    struct Options opts = {0};

    char *argv[] = {"file_name", NULL};
    int argc = sizeof(argv) / sizeof(argv[0]) - 1;  // NULL в argc не входит

    char *expected_argv[] = {"-"};
    int expected_file_count = sizeof(expected_argv) / sizeof(expected_argv[0]);
    
    ParseArgs(argc, argv, &opts);
    mu_assert_inteq(expected_file_count, opts.file_count);
    for (int i = 0; i < expected_file_count; i++)
        mu_assert_streq(expected_argv[i], opts.files[i]);

    OptionsFree(&opts);

    return 0;
}
