# Папка для объектников и бинарного файла
BUILD = build

# папка с исходниками
SRC = src

# имя бинарного файла
TARGET = cat

# компилятор
CC = gcc

# флаги компилятора
CFLAGS = -g -std=c11 -Wall -Werror -Wextra -MMD -MP

SRCS = $(wildcard $(SRC)/*.c)
OBJS = $(SRCS:$(SRC)/%.c=$(BUILD)/%.o)
DEPS = $(wildcard $(BUILD)/*.d)

# папка с файлами для тестирования
TEST_DATA = tests/data

TESTS     = test
TEST_SRCS = $(wildcard $(TESTS)/test_*.c)
TEST_BINS = $(TEST_SRCS:$(TESTS)/%.c=$(SUILD)/%)
TEST_OBJS = $(TEST_BINS:=.o)
LIB_OBJS  = $(filter-out $(BUILD)/main.o,$(OBJS))

# Проверка clang-format
CL = clang-format
CLN = -n
CLI = -i

# Проверка cppcheck
CPP = cppcheck
FLCPP = --enable=all --std=c11 --suppress=missingIncludeSystem --enable=warning,style

# Проверка valgrind
VL = valgrind
VLFL = --leak-check=full


.PHONY: all run test clean rebuild cln cli cpp val

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

rebuild: clean all

cln:
	$(CL) $(CLN) ./src/*.c ./src/*.h ./tests/*.c ./tests/*.h

cli:
	$(CL) $(CLI) ./src/*.c ./src/*.h ./tests/*.c ./tests/*.h

cpp:
	$(CPP) $(FLCPP) ./src/*.c ./src/*.h ./tests/*.c ./tests/*.h

val:
	$(VL) $(VLFL) ./$(BUILD)/$(TARGET) $(TEST_DATA)/simple.txt

-include $(DEPS)
