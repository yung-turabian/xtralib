# Variables
CC = gcc
CFLAGS = -Iinclude -Wall -Wextra -std=c11
SRC_DIR = src
INC_DIR = include/xtra
OBJ_DIR = obj
BIN_DIR = bin
LIB_DIR = lib
TESTS_DIR = tests
LIB_NAME = libXtra.a
LIB_PATH = /usr/local/lib
INC_PATH = /usr/local/include

# Source and object files
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC_FILES))
TEST_FILES = $(wildcard $(TESTS_DIR)/*.c)

# Target rules
all: $(LIB_DIR)/$(LIB_NAME)

$(LIB_DIR)/$(LIB_NAME): $(OBJ_FILES)
	@mkdir -p $(LIB_DIR)
	ar rcs $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Test runner
test: all
	$(CC) $(CFLAGS) $(TEST_FILES) $(LIB_DIR)/$(LIB_NAME) -o test_runner
	./test_runner


install: all
	@mkdir -p $(LIB_PATH) $(INC_PATH)
	cp $(LIB_DIR)/$(LIB_NAME) $(LIB_PATH)
	cp -r $(INC_DIR) $(INC_PATH)

uninstall:
	rm -f $(LIB_PATH)/$(LIB_NAME)
	rm -rf $(INC_PATH)/xtra

clean:
	rm -rf $(OBJ_DIR) $(LIB_DIR)
	rm -f test_runner

check:
	ar -t $(LIB_PATH)/$(LIB_NAME)


# Phony targets
.PHONY: all clean install uninstall
