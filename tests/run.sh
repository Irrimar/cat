#!/usr/bin/env bash
# Интеграционные тесты: запускаем build/cat и системный cat с одинаковыми
# аргументами и сравниваем stdout, stderr и код возврата побайтово.
# Эталон — всегда системный cat, ожидаемые результаты руками не пишем.
#
# Запуск: make test   (или bash tests/run.sh после make)

set -u   # ошибка при обращении к необъявленной переменной

cd "$(dirname "$0")/.." || exit 1     # работаем из корня проекта, пути короткие
MY_CAT="./build/cat"
SYS_CAT="$(command -v cat)"
DATA="tests/data"

TMP="$(mktemp -d)"
trap 'rm -rf "$TMP"' EXIT              # убрать временные файлы при любом выходе

export LC_ALL=C                        # сообщения об ошибках на английском у обеих программ

# Флаги, которые уже реализованы. Дописывай сюда по мере реализации,
# например: FLAGS="-n -b -s -E -T -nE -bs -nsT"
FLAGS=""

passed=0
failed=0

if [[ ! -x "$MY_CAT" ]]; then
    echo "run.sh: $MY_CAT не найден — сначала make" >&2
    exit 1
fi

# run_pair STDIN_FILE ARGS...
# Запускает обе программы с одинаковыми аргументами и stdin.
# Результаты кладёт в $TMP/{exp,act}.{out,err,rc} (expected / actual).
run_pair() {
    local stdin_file="$1"; shift
    "$SYS_CAT" "$@" <"$stdin_file" >"$TMP/exp.out" 2>"$TMP/exp.err"; echo $? >"$TMP/exp.rc"
    "$MY_CAT"  "$@" <"$stdin_file" >"$TMP/act.out" 2>"$TMP/act.err"; echo $? >"$TMP/act.rc"
    # Имя программы в stderr у всех разное (/bin/cat:, ./build/cat:) — приводим к "cat:"
    sed -i 's|^[^:]*cat: |cat: |' "$TMP/exp.err" "$TMP/act.err"
}

# report NAME — сравнивает результаты последнего run_pair и печатает diff при провале
report() {
    local name="$1" ok=1
    cmp -s "$TMP/exp.out" "$TMP/act.out" || ok=0
    cmp -s "$TMP/exp.err" "$TMP/act.err" || ok=0
    cmp -s "$TMP/exp.rc"  "$TMP/act.rc"  || ok=0

    if [[ $ok -eq 1 ]]; then
        passed=$((passed + 1))
        return
    fi

    failed=$((failed + 1))
    echo "FAIL: $name"
    echo "  rc: expected $(<"$TMP/exp.rc"), got $(<"$TMP/act.rc")"
    if ! cmp -s "$TMP/exp.out" "$TMP/act.out"; then
        echo "  stdout diff (< expected, > got):"
        diff "$TMP/exp.out" "$TMP/act.out" | head -20 | sed 's/^/    /'
    fi
    if ! cmp -s "$TMP/exp.err" "$TMP/act.err"; then
        echo "  stderr diff (< expected, > got):"
        diff "$TMP/exp.err" "$TMP/act.err" | head -20 | sed 's/^/    /'
    fi
}

# check ARGS...            — запуск с пустым stdin
check() {
    run_pair /dev/null "$@"
    report "cat $*"
}

# check_stdin FILE ARGS... — запуск с FILE на stdin
check_stdin() {
    local input="$1"; shift
    run_pair "$input" "$@"
    report "cat $* < $(basename "$input")"
}

# ---------------------------------------------------------------- тесты

# --- один файл, без флагов ---
for f in "$DATA"/*.txt; do
    check "$f"
done

# --- несколько файлов ---
check "$DATA/simple.txt" "$DATA/tabs.txt"
check "$DATA/no_trailing_newline.txt" "$DATA/simple.txt"        # склейка без \n
check "$DATA/empty.txt" "$DATA/simple.txt" "$DATA/empty.txt"

# --- stdin ---
check_stdin "$DATA/simple.txt"                                  # без аргументов
check_stdin "$DATA/simple.txt" -                                # явный "-"
check_stdin "$DATA/simple.txt" - -                              # явный "-"
check_stdin "$DATA/simple.txt" "$DATA/tabs.txt" - "$DATA/tabs.txt"
check_stdin "$DATA/simple.txt" - "$DATA/tabs.txt" "$DATA/tabs.txt" -

# --- ошибки ---
check "$DATA/nope.txt"                                          # нет файла → rc=1
check "$DATA/nope.txt" "$DATA/simple.txt"                       # ошибка, но остальные файлы выводятся
check "$DATA/simple.txt" "$DATA/nope.txt" "$DATA/simple.txt"
check "$DATA"                                                   # директория → "Is a directory"

# --- флаги ---
for flag in $FLAGS; do
    for f in "$DATA"/*.txt; do
        check "$flag" "$f"
    done
    check "$flag" "$DATA/simple.txt" "$DATA/blank_lines.txt"    # нумерация сквозная через файлы
    check_stdin "$DATA/blank_lines.txt" "$flag"
    check "$DATA/simple.txt" "$flag"                            # флаг после файла
done

# --help нельзя сравнить с системным cat (текст другой) — когда реализуешь,
# проверяй отдельно: rc=0 и непустой stdout.

# ---------------------------------------------------------------- итог

echo
echo "passed: $passed, failed: $failed"
[[ $failed -eq 0 ]]   # код возврата скрипта — для make test
