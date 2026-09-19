# Папка для объектников и бинарного файла
BUILD = build

# папка с исходниками
SRC = src

# имя бинарного файла
TARGET = cat

# компилятор
CC = gcc

# флаги компилятора
CFLAGS = -std=c11 -Wall -Werror -Wextra -MMD -MP

SRCS = $(wildcard $(SRC)/*.c)
OBJS = $(SRCS:$(SRC)/%.c=$(BUILD)/%.o)
DEPS = $(OBJS:.o=.d)

# папка с файлами для тестирования
TEST_DATA = tests/data

.PHONY: all run test clean

all: $(BUILD)/$(TARGET)

# линковка
$(BUILD)/$(TARGET): $(OBJS)
	$(CC) $^ -o $@

# компиляция
$(BUILD)/%.o: $(SRC)/%.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD):
	mkdir -p $@

run: all
	./$(BUILD)/$(TARGET) $(TEST_DATA)/single_line.txt # $(TEST_DATA)/simple.txt

test: all
	bash tests/run.sh

clean:
	rm -rf $(BUILD)

-include $(DEPS)
