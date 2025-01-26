CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Werror -Iinclude -O2

LIB_CURL = -lcurl
LIB_JSON-C = -ljson-c
LIB_TELEBOT = -L/usr/local/lib -ltelebot
LIB_BOOST = -lboost_filesystem

LDFLAGS = $(LIB_CURL) $(LIB_JSON-C) $(LIB_TELEBOT) $(LIB_BOOST)

# Project structure
SRC_DIR = src
INCLUDE_DIR = include

# Source files
BOT_SRC = $(wildcard $(SRC_DIR)/Bot/*.cpp)
SHELL_SRC = $(wildcard $(SRC_DIR)/Shell/*.cpp)
CORE_SRC = $(wildcard $(SRC_DIR)/*.cpp)
SRC = $(BOT_SRC) $(SHELL_SRC) $(CORE_SRC)

# Object files (placed next to source files)
OBJS = $(patsubst %.cpp,%.o,$(SRC))

# Output binary
TARGET = trat_bot

# Rules
.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

# Rule to compile .cpp files into .o files (object files are placed next to sources)
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	find $(SRC_DIR) -name "*.o" -type f -delete
	rm -f $(TARGET)