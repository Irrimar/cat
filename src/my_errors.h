#ifndef MY_ERRORS_H
#define MY_ERRORS_H

/* Внутренние коды ошибок.

   Наружу не выходят: main возвращает 0 при успехе и 1 при любой ошибке,
   как настоящий cat. Эти коды нужны только внутри программы, чтобы
   различать причину сбоя. */

#define SUCCESS 0             // ошибок нет
#define MEMMORY_ERROR 1       // calloc в ParseArgs вернул NULL
#define UNKNOWN_FLAG_ERROR 2  // в аргументах встретился неизвестный флаг
#define INPUT_ERROR 3         // не удалось открыть файл (причина в errno)

#endif