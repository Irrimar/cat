#define _GNU_SOURCE

#include "../src/cat.h"
#include "minunit.h"

static int test_copies_input(void) {
    char input[] = "abc";
    int size = strlen(input);
    FILE *in = fmemopen(input, size, "r");
    if (!in) {
        perror("in");
        return 1;
    }

    FILE *tmp = tmpfile();
    if (!tmp) {
        perror("tmpfile");
        fclose(in);
        return 1;
    }

    FileProcessing(in, tmp);

    rewind(tmp);
    char buf[64];
    size = fread(buf, 1, sizeof(buf) - 1, tmp);
    buf[size] = '\0';

    fclose(tmp);
    fclose(in);

    mu_assert_streq(input, buf);
    return 0;
}

int main(void) {
    mu_run_test(test_copies_input);
    /* ... остальные */
    mu_report();
    return mu_tests_failed != 0;
}
