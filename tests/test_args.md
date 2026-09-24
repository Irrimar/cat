# Тесты из `tests/test_args.c`

Всего 43 теста. Запуск: `make test`

Проверяемые функции: `ParseArgs`, `OpenInput`, `CloseInput`, `OptionsFree` — объявлены в [src/args.h](../src/args.h).

В каждом тесте `ParseArgs` проверяется по одной схеме:

- код возврата (0 — разбор прошёл, 2 — неизвестный флаг);
- все шесть полей флагов структуры `Options`;
- при коде 0 — количество файлов и их имена по порядку.

При коде возврата 2 список файлов не проверяется: разбор прерван, и его содержимое не является частью контракта.

## 1. Только имена файлов и `-`

| Тест | Аргументы (без `argv[0]`) | res | Ожидаемые файлы |
|------|---------------------------|-----|-----------------|
| `test_null_options` | (нет) | 0 | `-` |
| `test_dash_options` | `-` | 0 | `-` |
| `test_two_dash_options` | `- -` | 0 | `-` `-` |
| `test_one_file_name_options` | `filename1` | 0 | `filename1` |
| `test_two_file_names_options` | `filename1 filename2` | 0 | `filename1` `filename2` |
| `test_dushes_file_names_options` | `- filename1 - filename2 -` | 0 | `-` `filename1` `-` `filename2` `-` |
| `test_file_names_dushes_options` | `filename1 - filename2` | 0 | `filename1` `-` `filename2` |

Проверяют: файлов нет → подставляется `-` (stdin); `-` попадает в список как обычный элемент; порядок файлов сохраняется.

## 2. Только флаги

| Тест | Аргументы | res | Что проверяет |
|------|-----------|-----|---------------|
| `test_all1_options` | `-n -b -s -E -T --help` | 0 | все флаги по отдельности |
| `test_all2_options` | `-nbsET` | 0 | все флаги склеены в один аргумент |
| `test_all3_options` | `-nb -sET` | 0 | флаги склеены в две группы |
| `test_all4_options` | `-t` | 2 | неизвестный флаг (`-t` вместо `-T`) |
| `test_all5_options` | `-n -t` | 2 | верный флаг, затем неизвестный |
| `test_all6_options` | `-n -t -n` | 2 | разбор прерывается, остальное не читается |
| `test_all7_options` | `-t -n` | 2 | неизвестный первым, верный после него не читается |
| `test_all8_options` | `-N -B -S -e -t --help` | 2 | все флаги в неверном регистре |
| `test_all9_options` | `-nbsEt` | 2 | неизвестный флаг в конце склейки |

## 3. Файлы и флаги вместе

| Тест | Аргументы | res | Ожидаемые файлы | Что проверяет |
|------|-----------|-----|-----------------|---------------|
| `test_files1_and_options` | `-n a.txt` | 0 | `a.txt` | флаг перед файлом |
| `test_files2_and_options` | `a.txt -n` | 0 | `a.txt` | флаг после файла |
| `test_files3_and_options` | `a.txt -n b.txt` | 0 | `a.txt` `b.txt` | флаг между файлами |
| `test_files4_and_options` | `-n a.txt -E b.txt -T` | 0 | `a.txt` `b.txt` | всё вперемешку |
| `test_files5_and_options` | `-nE a.txt b.txt` | 0 | `a.txt` `b.txt` | склеенные флаги |
| `test_files6_and_options` | `-nbsET a.txt` | 0 | `a.txt` | все флаги сразу |
| `test_files7_and_options` | `-n a.txt - b.txt` | 0 | `a.txt` `-` `b.txt` | stdin среди файлов |
| `test_files8_and_options` | `- -n` | 0 | `-` | stdin перед флагом |
| `test_files9_and_options` | `-n` | 0 | `-` | только флаги |
| `test_files10_and_options` | `--help a.txt` | 0 | `a.txt` | `--help` с файлом |
| `test_files11_and_options` | `a.txt -z b.txt` | 2 | не проверяются | неизвестный флаг |
| `test_files12_and_options` | `-n2 a.txt` | 2 | не проверяются | мусор в склейке |
| `test_files13_and_options` | `-n -n a.txt` | 0 | `a.txt` | повтор флага |
| `test_files14_and_options` | `-nn a.txt` | 0 | `a.txt` | повтор в склейке |
| `test_files15_and_options` | `-help a.txt` | 2 | не проверяются | `-help`, а не `--help` |
| `test_files16_and_options` | `--number` | 2 | не проверяются | длинный флаг не из списка |
| `test_files17_and_options` | `""` (пустая строка) | 0 | `""` | пустое имя файла |
| `test_files18_and_options` | `-- a.txt` | 2 | не проверяются | `--` не поддерживаем |

## 4. `OpenInput` и `CloseInput`

| Тест | Что проверяет |
|------|---------------|
| `test_open_input_dash` | `OpenInput("-")` возвращает именно `stdin`, а не открывает файл с именем `-` |
| `test_open_input_file` | существующий файл открывается, содержимое читается с начала |
| `test_open_input_missing` | несуществующий файл → `NULL` |
| `test_close_input_keeps_stdin` | `CloseInput` не закрывает `stdin`: чтение продолжается дальше |
| `test_close_input_clears_eof` | `CloseInput` сбрасывает признак EOF у `stdin` (нужно для `cat - file -`) |
| `test_close_input_closes_file` | обычный файл действительно закрывается: 2000 циклов открыть/закрыть не исчерпывают лимит дескрипторов |

Тесты этого раздела читают `tests/data/single_line.txt` по относительному пути, поэтому запускать их нужно из корня проекта (`make test` так и делает). Тесты `stdin` подменяют поток через `freopen`.

## 5. `OptionsFree`

| Тест | Что проверяет |
|------|---------------|
| `test_options_free_clears_pointer` | после освобождения `opts.files` обнулён, `opts.file_count` сброшен в 0 |
| `test_options_free_twice` | повторный вызов безопасен: второй `free` получает уже `NULL`, структура остаётся согласованной |
| `test_options_free_zeroed` | вызов на нетронутой структуре `{0}` не падает: `free(NULL)` законен |

`OptionsFree` обнуляет и указатель, и счётчик, поэтому после неё структуру нельзя случайно обойти по `file_count` — цикл просто не выполнится.

## Чего пока нет

- Поведение при неудачном `calloc` (`res = 1`) — воспроизвести без подмены аллокатора нельзя.
