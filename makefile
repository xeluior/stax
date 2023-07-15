SHELL = /bin/sh
CC := clang
SRC_DIR := src
OBJ_DIR := build
BIN_DIR := bin
EXE := $(BIN_DIR)/stax
SRC := $(wildcard $(SRC_DIR)/*.c)
OBJ := $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
CPPFLAGS := -Iinclude -I/usr/include/SDL2 -I/usr/include/harfbuzz -I/usr/include/glib-2.0 -I/usr/lib64/glib-2.0/include -I/usr/include/sysprof-4 -I/usr/include/freetype2 -I/usr/include/libpng16 -pthread
CFLAGS := -Wall -Wextra -Werror
LDLIBS := -lSDL2 -lSDL2_ttf

.PHONY: all clean

all: $(EXE)

$(EXE): $(OBJ) | $(BIN_DIR)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(BIN_DIR) $(OBJ_DIR):
	mkdir -p $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

debug: CFLAGS += -g
debug: $(EXE)

clean:
	rm -rf $(BIN_DIR) $(OBJ_DIR)
