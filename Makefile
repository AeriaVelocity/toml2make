PROJECT_NAME = toml2make
VERSION = 0.1.0
LICENSE = MIT
DESCRIPTION = A tool to convert TOML files to Makefiles, for C/C++ projects.

CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic
CVERSION = c17

SRC_DIR = src
BUILD_DIR = build
BIN_DIR = bin
TARGET = $(BIN_DIR)/$(PROJECT_NAME)

SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(SRC:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

prefix ?= /usr/local

all: $(TARGET)

$(TARGET): $(OBJ)
	@mkdir -p $(BIN_DIR)
	$(CC) -o $@ $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

run: all
	$(TARGET)

# Clean up build artifacts
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

# Install the binary
install: all
	@mkdir -p $(prefix)/bin
	cp $(TARGET) $(prefix)/bin/

# Uninstall the binary
uninstall:
	rm -f $(prefix)/bin/$(PROJECT_NAME)

.PHONY: all run clean install uninstall
