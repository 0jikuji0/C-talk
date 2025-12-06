# ==============================
# C-Talk Project Makefile
# ==============================

# Compiler and flags
CC      := gcc
CFLAGS  := -Wall -Wextra -Werror -std=c99 -I include -lgmp
LDFLAGS :=

# Directories
SRC_DIR := src
BUILD_DIR := build

# Executable name
TARGET := $(BUILD_DIR)/b.out

# Source files
SRC := $(SRC_DIR)/c_talk.c \
       $(SRC_DIR)/network.c \
       $(SRC_DIR)/client.c \
       $(SRC_DIR)/server.c \
	   $(SRC_DIR)/crypto.c

# Object files (in build/)
OBJ := $(SRC:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

# Default target
all: $(TARGET)

# Link step
$(TARGET): $(OBJ)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(OBJ) -o $@ $(LDFLAGS) -lgmp
	@echo "✅ Build complete: $@"

# Compile step
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compiled: $<"

# Clean up
clean:
	rm -rf $(BUILD_DIR)
	@echo "🧹 Cleaned build directory."

# Run the program
run: all
	./$(TARGET)

# Phony targets (not actual files)
.PHONY: all clean run
