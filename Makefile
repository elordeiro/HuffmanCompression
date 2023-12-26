# Compiler and compiler flags
CC = gcc
CFLAGS = -Wall -Wextra -g

# Source files and output executable
SRC = huffman.c heap.c vector.c 
EXE = huffman

# Default target
all: $(EXE)

# Compile the source files and link them to create the executable
$(EXE): $(SRC)
	$(CC) $(CFLAGS) -o $(EXE) $(SRC)

# Clean up the generated files
clean:
	rm -f $(EXE) *.o *.out
	rm -rf *.dSYM

# Define a "run" target to execute the program
run: $(EXE)
	./$(EXE)