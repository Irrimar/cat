#ifndef MY_ERRORS_H
#define MY_ERRORS_H

/* Внутренние коды возврата.

   Наружу не выходят: main возвращает 0 при успехе и 1 при любой ошибке,
   как настоящий cat. Эти коды нужны только внутри программы, чтобы
   различать причину сбоя.

   HELP_REQUESTED — не сбой: разбор прекращён, потому что запрошена справка. */
#define OUTPUT_ERROR 5  // не удалось записать в stdout (причина в errno)

#define SUCCESS 0             // ошибок нет
#define MEMMORY_ERROR 1       // calloc в ParseArgs вернул NULL
#define UNKNOWN_FLAG_ERROR 2  // в аргументах встретился неизвестный флаг
#define INPUT_ERROR 3         // не удалось открыть файл (причина в errno)
#define HELP_REQUESTED 4  // не ошибка: запрошена справка, разбор прекращён

#endif