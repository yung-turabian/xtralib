CC = clang
AR = ar
EMCC = emcc
EMAR = emar
CFLAGS = -Iinclude -D_GNU_SOURCE -Wall -Wextra -std=c11
EMCC_FLAGS = -Iinclude -D_GNU_SOURCE -Wall -Wextra -std=c11 -pthread -I/usr/local/include 
SRC_DIR = src
INC_DIR = include/*
OBJ_DIR = obj
BIN_DIR = bin
LIB_DIR = lib
TESTS_DIR = tests
LIB_NAME = libreis.a
LIB_PATH1 = /usr/local/lib
LIB_PATH2 = /usr/lib
INC_PATH = /usr/local/include

ARCH := $(shell uname -s)
$(info ARCH=$(ARCH))

HAVE_RAYLIB := $(shell ld -lraylib 2>/dev/null && echo yes || echo no)
ifeq ($(HAVE_RAYLIB), yes)
    CFLAGS += -lraylib
    SRC_FILES += $(SRC_DIR)/raylib.c
endif

SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC_FILES))
WASM_OBJ_FILES = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.wasm.o, $(SRC_FILES))
TEST_FILES = $(wildcard $(TESTS_DIR)/*.c)

all: $(LIB_DIR)/$(LIB_NAME)

$(LIB_DIR)/$(LIB_NAME): $(OBJ_FILES)
	@mkdir -p $(LIB_DIR)
	$(AR) rcs $@ $^

$(LIB_DIR)/$(LIB_NAME).wasm: $(WASM_OBJ_FILES)
	@mkdir -p $(LIB_DIR)
	$(EMAR) rcs $@ $^ 

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.wasm.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(EMCC) $(EMCC_FLAGS) --target=wasm32 -c $< -o $@

test: all
	$(CC) $(CFLAGS) $(TEST_FILES) $(LIB_DIR)/$(LIB_NAME) -o test_runner
	./test_runner


install: all
	@mkdir -p $(LIB_PATH) $(INC_PATH)
	sudo cp $(LIB_DIR)/$(LIB_NAME) $(LIB_PATH1)

ifeq ($(ARCH),Linux) 
	sudo cp $(LIB_DIR)/$(LIB_NAME) $(LIB_PATH2)
endif

	sudo cp -r $(INC_DIR) $(INC_PATH)

uninstall:
	rm -f $(LIB_PATH1)/$(LIB_NAME)
	rm -f $(LIB_PATH2)/$(LIB_NAME)
	rm -rf $(INC_PATH)/reis

clean:
	rm -rf $(OBJ_DIR) $(LIB_DIR)
	rm -f test_runner

check:
	$(AR) -t $(LIB_PATH1)/$(LIB_NAME)
	$(AR) -t $(LIB_PATH2)/$(LIB_NAME)

wasm: $(LIB_DIR)/$(LIB_NAME).wasm


.PHONY: all clean install uninstall
