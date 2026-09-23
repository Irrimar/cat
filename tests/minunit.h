#ifndef MINUNIT_H
#define MINUNIT_H

#include <stdio.h>
#include <string.h>

static int mu_tests_run = 0;
static int mu_tests_failed = 0;
static const char *mu_current_test = "";

#define mu_assert(message, test)                                                           \
    do {                                                                                   \
        if (!(test)) {                                                                     \
            printf("FAIL %s (%s:%d): %s\n", mu_current_test, __FILE__, __LINE__, message); \
            return 1;                                                                      \
        }                                                                                  \
    } while (0)

#define mu_assert_streq(expected, actual)                     \
    do {                                                      \
        const char *e_ = (expected), *a_ = (actual);          \
        if (strcmp(e_, a_) != 0) {                            \
            printf(                                           \
                "FAIL %s (%s:%d): strings differ\n"           \
                "    expected: \"%s\"\n"                      \
                "    actual:   \"%s\"\n",                     \
                mu_current_test, __FILE__, __LINE__, e_, a_); \
            return 1;                                         \
        }                                                     \
    } while (0)

#define mu_assert_inteq(expected, actual)                     \
    do {                                                      \
        size_t e_ = (expected), a_ = (actual);                \
        if (e_ != a_) {                                       \
            printf(                                           \
                "FAIL %s (%s:%d): integers differ\n"          \
                "    expected: %zu\n"                         \
                "    actual:   %zu\n",                        \
                mu_current_test, __FILE__, __LINE__, e_, a_); \
            return 1;                                         \
        }                                                     \
    } while (0)

#define mu_run_test(test)                   \
    do {                                    \
        mu_current_test = #test;            \
        mu_tests_run++;                     \
        if (test() != 0) mu_tests_failed++; \
    } while (0)

#define mu_report() printf("%s: %d tests, %d failed\n", __FILE__, mu_tests_run, mu_tests_failed)

#endif /* MINUNIT_H */