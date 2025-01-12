#ifndef MAIN_HPP
#define MAIN_HPP

#include <iostream>
#include <cstdlib>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <telebot.h>
#include <filesystem>
#include <thread>
#include "bot.hpp"
#include "shell.hpp"
#include "networking.hpp"
#include "parser.hpp"

#define CURRENT_PATH std::filesystem::current_path().string().c_str()

#endif
