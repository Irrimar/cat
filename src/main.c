#include <stdio.h>
#include <string.h>  // Используются для получения ошибок 
#include <errno.h>   // как в cat

#include "cat.h"

int main(int argc, char *argv[]) {
    // for (int  i = 0; i < argc; i++) {
    //     printf("argc = %d\nargv[%d] = %s\n", argc, i, argv[i]);
    // }

    // parse_args

    /*for (int i = 1; i < argc; i++) {
       if (argv[i][0] == '-' && argv[i][1] != '\0') {
            // флаг: argv[i][1] — буква ('n', 'b', 's'...)
            // или strcmp(argv[i], "--help") == 0
        } else {
            // имя файла (или "-" — stdin)
        }*/

       
        int flag_err = 0;
        for (int i = 1; i < argc; i++) {
            char *path_to_file = argv[i];
            FILE *file_to_read = fopen(path_to_file, "r");

            // printf("Файл [ %s ] \n\n", path_to_file);

            if (file_to_read == NULL) {
                fprintf(stderr, "cat: %s: %s\n", path_to_file, strerror(errno));
                flag_err = 1;
            } else {
                // printf("Файл [ %s ] успешно открыт\n\n", path_to_file);

                PrintFile(file_to_read);
                fclose(file_to_read);
            }
        }

    return flag_err;
}