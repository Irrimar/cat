# Папка для объектников и бинарного файла
BUILD=build

# папка с исходноком
SRC=src

# имя бинарного файла
TARGET=main

# компилятор
GCC=gcc

# флаги компилятора
GCCFLAGS=-std=c11 -Wall -Werror -Wextra

# папка с файлами для тестирования
TEST_DATA=tests/data

# тестовые файлы
T1=blank_lines.txt
T2=cyrillic.txt
T3=empty.txt
T4=mixed.txt
T5=no_trailing_newline.txt
T6=only_newlines.txt
T7=simple.txt
T8=single_line.txt
T9=tabs.txt

all:
	$(GCC) $(GCCFLAGS) $(SRC)/$(TARGET).c -o $(BUILD)/$(TARGET)
	@./$(BUILD)/$(TARGET) $(TEST_DATA)/$(T8)