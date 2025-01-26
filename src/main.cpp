/* trat/main.cpp */
#include <iostream>
#include <cstdlib>
#include <stdlib.h>
#include <unistd.h>
#include <telebot.h>
#include "../include/bot.hpp"
#include "../include/parser.hpp"

#define CURRENT_PATH std::filesystem::current_path().string().c_str()


int main() 
{ 
  int64_t chat_id = 6479143326;
  const char* token_buffer =  "7843656436:AAH6rROv5vGc2FiE16lGgD4-6A6OvbruW50";
  trat::Bot bot = trat::Bot(token_buffer, chat_id);
  bot.work();
  
  return 0;

}
