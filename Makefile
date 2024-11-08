# Variables
CC = clang
AR = ar
EMCC = emcc
EMAR = emar
CFLAGS = -Iinclude -D_GNU_SOURCE -Wall -Wextra -std=c11 -lraylib
EMCC_FLAGS = -Iinclude -D_GNU_SOURCE -Wall -Wextra -std=c11 -pthread -I/usr/local/include 
SRC_DIR = src
INC_DIR = include/xtra
OBJ_DIR = obj
BIN_DIR = bin
LIB_DIR = lib
TESTS_DIR = tests
LIB_NAME = libxtra.a
LIB_PATH1 = /usr/local/lib
LIB_PATH2 = /usr/lib
INC_PATH = /usr/local/include

ARCH := $(shell uname -s)
$(info ARCH=$(ARCH))

# Source and object files
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC_FILES))
WASM_OBJ_FILES = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.wasm.o, $(SRC_FILES))
TEST_FILES = $(wildcard $(TESTS_DIR)/*.c)

# Target rules
all: $(LIB_DIR)/$(LIB_NAME)

# Library targets
$(LIB_DIR)/$(LIB_NAME): $(OBJ_FILES)
	@mkdir -p $(LIB_DIR)
	$(AR) rcs $@ $^

$(LIB_DIR)/$(LIB_NAME).wasm: $(WASM_OBJ_FILES)
	@mkdir -p $(LIB_DIR)
	$(EMAR) rcs $@ $^ 

# Object file rules
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.wasm.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(EMCC) $(EMCC_FLAGS) --target=wasm32 -c $< -o $@

# Test runner
test: all
	$(CC) $(CFLAGS) $(TEST_FILES) $(LIB_DIR)/$(LIB_NAME) -o test_runner
	./test_runner


install: all
	@mkdir -p $(LIB_PATH) $(INC_PATH)
	cp $(LIB_DIR)/$(LIB_NAME) $(LIB_PATH1)

ifeq ($(ARCH),Linux) 
	cp $(LIB_DIR)/$(LIB_NAME) $(LIB_PATH2)
endif

	cp -r $(INC_DIR) $(INC_PATH)

uninstall:
	rm -f $(LIB_PATH1)/$(LIB_NAME)
	rm -f $(LIB_PATH2)/$(LIB_NAME)
	rm -rf $(INC_PATH)/xtra

clean:
	rm -rf $(OBJ_DIR) $(LIB_DIR)
	rm -f test_runner

check:
	$(AR) -t $(LIB_PATH1)/$(LIB_NAME)
	$(AR) -t $(LIB_PATH2)/$(LIB_NAME)

# WebAssembly target
wasm: $(LIB_DIR)/$(LIB_NAME).wasm


# Phony targets
.PHONY: all clean install uninstall
