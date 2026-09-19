#include <stdio.h>
#include <string.h>
#include <errno.h>

// parse_args

int main(int argc, char *argv[]) {
    for (int  i = 0; i < argc; i++) {
        printf("argv[%d] = %s\n", i, argv[i]);
    }

    char *file_path = "tests/data/single_line.txt";
    printf("\nфайл [ %s ]\n\n", file_path);

    FILE *file_read = fopen(file_path, "r");
    if (file_read == NULL) {
        fprintf(stderr, "файл => [ %s ]: %s\n", file_path, strerror(errno));
        return 1;
    }
    printf("Файл [ %s ] успешно открыт\n\n", file_path);

    int ch;
    while((ch = fgetc(file_read))!= EOF) {
        putchar(ch);
    }

    /*for (int i = 1; i < argc; i++) {
       if (argv[i][0] == '-' && argv[i][1] != '\0') {
            // флаг: argv[i][1] — буква ('n', 'b', 's'...)
            // или strcmp(argv[i], "--help") == 0
        } else {
            // имя файла (или "-" — stdin)
        }*/

    fclose(file_read);
    return 0;
}