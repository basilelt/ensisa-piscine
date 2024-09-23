# Project Makefile

# Variables
BUILD_DIR=build

CC=gcc

CFLAGS=-Wall -Wextra -std=c11 -O2 -g -Iinclude `pkg-config --cflags gtk+-3.0` -MMD -O3
LDFLAGS=`pkg-config --libs gtk+-3.0`

SRC_DIR=src
OBJ_DIR=$(BUILD_DIR)/obj
BIN_DIR=$(BUILD_DIR)/bin

EXECUTABLE=$(BIN_DIR)/game
TEST_EXECUTABLE=$(BIN_DIR)/tests
TEST_DIR=tests
DOCS_DIR=docs/html

# Source files
SRCS=$(wildcard $(SRC_DIR)/*.c)
OBJS=$(filter-out $(OBJ_DIR)/game.o, $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o))

# Targets
all: clean $(EXECUTABLE) docs tests
#all: clean $(EXECUTABLE)

$(EXECUTABLE): $(OBJS) $(OBJ_DIR)/game.o
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $(OBJS) $(OBJ_DIR)/game.o -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Compile tests.c into an object file in obj directory
$(OBJ_DIR)/tests.o: $(TEST_DIR)/tests.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Link the test executable from object files
$(TEST_EXECUTABLE): $(OBJ_DIR)/tests.o $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

tests: $(TEST_EXECUTABLE)
	./$(TEST_EXECUTABLE)

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR) $(DOCS_DIR) $(TEST_EXECUTABLE) $(EXECUTABLE).dSYM

docs:
	doxygen -q Doxyfile
	mkdir -p docs/html/project/docs/img/
	cp docs/img/* docs/html/project/docs/img/

.PHONY: all clean tests docs
