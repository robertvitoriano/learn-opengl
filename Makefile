# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Iinclude -I/usr/include
LDFLAGS = -lSDL2 -lGL -ldl

# Source files and object files
SRC = src/main.cpp src/glad.c
OBJ = bin/main.o bin/glad.o
EXEC = build/Triangle

# Default target to build the application
all: create_bin_dir $(EXEC)

# Rule to create the bin directory if it doesn't exist
create_bin_dir:
	mkdir -p bin

# Rule to compile main.cpp to main.o
bin/main.o: src/main.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Rule to compile glad.c to glad.o
bin/glad.o: src/glad.c
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Rule to link object files and create the executable
$(EXEC): $(OBJ)
	$(CXX) $(OBJ) -o $(EXEC) $(LDFLAGS)

# Clean up object files and executable
clean:
	rm -f $(OBJ) $(EXEC)
	rm -rf bin
