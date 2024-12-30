# Define the compiler and flags
CC = cc
CFLAGS = -Wall -Wextra -pedantic -ggdb -lm
INCLUDE_PATH = -Ilib
OUTPUT = run

# Define source files and object files
SRC = src/inc.c src/grid.c src/queue.c src/search.c
OBJ = $(SRC:.c=.o)

# Default target
all: $(OUTPUT)

# Rule for linking the output executable
$(OUTPUT): $(OBJ)
	$(CC) $(OBJ) -o $(OUTPUT) $(CFLAGS) $(INCLUDE_PATH)

# Rule for compiling .c to .o
%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS) $(INCLUDE_PATH)

# Clean up object files and the executable
clean:
	rm -f $(OBJ) $(OUTPUT)

# Phony targets (not actual files)
.PHONY: all clean
