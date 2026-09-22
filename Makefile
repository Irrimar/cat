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

TESTS     = tests
TEST_SRCS = $(wildcard $(TESTS)/test_*.c)
TEST_BINS = $(TEST_SRCS:$(TESTS)/%.c=$(BUILD)/%)
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
# Основная программа
# линковка
$(BUILD)/$(TARGET): $(OBJS)
	$(CC) $^ -o $@

# компиляция
$(BUILD)/%.o: $(SRC)/%.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD):
	mkdir -p $@

# Тесты
# юнит-тесты: компиляция и линковка с объектниками проекта (без main.o)
$(BUILD)/test_%.o: $(TESTS)/test_%.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/test_%: $(BUILD)/test_%.o $(LIB_OBJS)
	$(CC) $^ -o $@

.SECONDARY: $(TEST_OBJS)

run: all
	./$(BUILD)/$(TARGET) - -
	# $(TEST_DATA)/single_line.txt -
	# $(TEST_DATA)/simple.txt

test: all $(TEST_BINS)
	@for t in $(TEST_BINS); do ./$$t || exit 1; done
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
