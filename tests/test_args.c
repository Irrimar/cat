#include "minunit.h"

/* заглушка, пока args.c пуст */
static int test_placeholder(void) {
    mu_assert("placeholder", 1);
    return 0;
}

int main(void) {
    mu_run_test(test_placeholder);
    mu_report();
    return mu_tests_failed != 0;
}
