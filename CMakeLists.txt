cmake_minimum_required(VERSION 3.10)

project(T-RAT VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Specify the output directory for the executable
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

include_directories(include)

file(GLOB BOT_SOURCES
    src/Bot/*.cpp
    src/*.cpp
    src/Shell/*.cpp
)

# Find and link Boost libraries
find_package(Boost REQUIRED COMPONENTS system filesystem)
if (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    message(STATUS "Boost found: ${Boost_VERSION}")
else()
    message(FATAL_ERROR "Boost not found")
endif()

find_package(CURL REQUIRED)
if (CURL_FOUND)
    include_directories(${CURL_INCLUDE_DIRS})
    message(STATUS "libcurl found: ${CURL_VERSION}")
else()
    message(FATAL_ERROR "libcurl not found")
endif()

find_library(TELEBOT_C_LIBRARY telebot REQUIRED)
if (TELEBOT_C_LIBRARY)
    message(STATUS "Telebot-C library found")
else()
    message(FATAL_ERROR "Telebot-C library not found")
endif()

add_executable(trat_bot ${BOT_SOURCES})

target_link_libraries(trat_bot
    ${Boost_LIBRARIES}
    ${CURL_LIBRARIES}
    ${TELEBOT_C_LIBRARY}
)
