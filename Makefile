# Makefile to compile and link the bot application

# Compiler and flags
CC = g++                              # C++ compiler
CFLAGS =  -g -Wall -Werror  -static -std=c++17 -Oz #-fsanitize=leak            # Compiler flags (warnings as errors)
INCLUDE = -I/usr/local/include        # Include directories
LFLAGS = -L/usr/local/lib -ltelebot -ljson-c -lcurl -lboost_system # Linker flags

# Source files and object files
SRC = $(wildcard src/*.cpp)  $(wildcard src/Bot/*.cpp) $(wildcard src/Shell/*.cpp)  # $(wildcard src/Emailer*.cpp)      # Find all .cpp files in src/
OBJ = $(SRC:.cpp=.o)                  # Convert .cpp files to .o object files
EXE = bot                             # Output executable

# Default target to build the executable
all: $(EXE)

# Rule to link object files into the executable
$(EXE): $(OBJ)
	$(CC) $(OBJ) -o $(EXE) $(LFLAGS)

# Rule to compile .cpp files to .o object files
%.o: %.cpp
	$(CC) -c $< -o $@ $(INCLUDE) $(CFLAGS)

# Clean up generated files
clean:
	rm -f $(OBJ) $(EXE)

# Optional: A target to install
install: $(EXE)
	cp $(EXE) /usr/local/bin

# Phony targets are not actual files
.PHONY: all clean install
