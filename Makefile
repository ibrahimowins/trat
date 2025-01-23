# Compiler and flags
CXX = g++
CXXFLAGS = -static -std=c++17 -Werror -Wall -Iinclude

# Libraries and includes
BOOST_LIBS = -lboost_system -lboost_filesystem
CURL_LIBS = -lcurl
OPENSSL_LIBS = -lssl -lcrypto
TELEBOT_LIBS = -ltelebot

# Output and directories
TARGET = trat_bot
SRC_DIR = src
BOT_SOURCES = $(wildcard $(SRC_DIR)/Bot/*.cpp)
SHELL_SOURCES = $(wildcard $(SRC_DIR)/Shell/*.cpp)
MAIN_SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
SOURCES = $(BOT_SOURCES) $(SHELL_SOURCES) $(MAIN_SOURCES)
OBJECTS = $(SOURCES:.cpp=.o)

# Default target
all: $(TARGET)

# Build target
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(BOOST_LIBS) $(CURL_LIBS) $(OPENSSL_LIBS) $(TELEBOT_LIBS)

# Compile sources into objects
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build files
clean:
	rm -f $(OBJECTS) $(TARGET)

# Debug target
debug: CXXFLAGS += -g
debug: clean $(TARGET)

# Phony targets
.PHONY: all clean debug

