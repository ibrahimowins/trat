#ifndef BOT_HPP
#define BOT_HPP

#include <iostream>
#include <string>
#include <filesystem>
#include <telebot.h>
#include <cstdint>
#include <cstring>  // For strdup
#include <stdexcept> // For exceptions
#include <thread>
#include "shell.hpp"
#include "networking.hpp"
#include "parser.hpp"
namespace trat {

class Shell;

class Bot
{
public:
    telebot_handler_t handle;  
    char* token;                 
    int64_t clientId;
    Shell shell;                          

    Bot(const char* Token, const int64_t& ClientId); 
    ~Bot();                                          
    bool sendMessage(const char* Message);  
    bool sendPhoto(const char* FilePath);
    bool sendDocument(const char* DocumentPath);

    bool downloadFromChat(const char* FileId, const char* FilePath);
    void listen();
};

}  // namespace trat

#endif  // BOT_HPP
