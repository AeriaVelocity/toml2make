# Makefile generated by TOML To Makefile.
# Generated on 2024-10-31 at 12:55:12 UTC.
# For more information, see <https://github.com/AeriaVelocity/toml2make>

NAME = toml2make
VERSION = 0.3.0
LICENSE = MIT OR GPL-3.0-or-later
DESCRIPTION = A tool to convert TOML files to Makefiles, for C/C++ projects.
AUTHOR = Arsalan 'Aeri' Kazmi

CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic --std=gnu17
CVERSION = gnu17

SRC_DIR = src
BUILD_DIR = build
INSTALL_DIR = ~/.local/bin/
INCLUDE_DIR = include
BIN_DIR = bin
TARGET = $(BIN_DIR)/$(NAME)

SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(SRC:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	@mkdir -p $(BIN_DIR)
	$(CC) -I $(INCLUDE_DIR) -o $@ $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) -I $(INCLUDE_DIR) $(CFLAGS) -c $< -o $@

run: all
	./$(TARGET)

install:
	mkdir -p $(INSTALL_DIR)
	cp $(TARGET) $(INSTALL_DIR)

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

.PHONY: all run clean
