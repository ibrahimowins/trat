/* trat/src/Bot/handling.cpp */

#include "../../include/bot.hpp"
#include <cstring>
#include <string>

namespace trat
{
  bool Bot::checkIfCommand(const char* Message) 
  {
    if(!Message) {  return false; }

    char* prefix = parser::getPrefixFromString(Message, ' ');
    if (!prefix) { return false; }
    
    for (size_t i = 0; i < NUMBER_RECOGNIZED_COMMANDS; ++i)
    {
      if ( (strcmp(prefix, (this -> commands)[i] )) == 0)
      {
        parser::cleanString(prefix);
        return true;
      }
    }
    return false;
  }
  
  void Bot::handleTextBasedCommand(const char* Telegram_Message_Text, const char* Command,  char* Shell_Function_Callback_Result)
  {
    if( (!Telegram_Message_Text) || (!Command) || (!Shell_Function_Callback_Result)) { return; }

    if (strstr(Telegram_Message_Text, Command))
    {
      /* Aliasing  the pointer */
      char* function_result = Shell_Function_Callback_Result;
      this->sendMessage(function_result);
      parser::cleanString(function_result);
    }
  }
  
  void Bot::handleDownloadCommand(const char* Telegram_Message_Text)
  {
    char* link = parser::checkCommandAndExtractParemeter("/download", Telegram_Message_Text);
    if(!link)
    {
      this -> sendMessage("Failed to extract link");
      return;
    }
    if(!(networking::checkLinkValidity(link))) 
    {
      this -> sendMessage("Invalid link");
      parser::cleanString(link);
      return;
    }
    parser::cleanString(link);
    
    char* file_name = parser::getSuffixFromString(link, '/');
    if (!file_name)
    {
      this -> sendMessage("Failed to extract the file name from the provided link");
    }
    char* current_path = parser::copyString(CURRENT_PATH);
    char* file_path = parser::constructFilePath(current_path, file_name); 
    if(!file_path)
    {
      this -> sendMessage("Failed to construct file path");
      parser::cleanString(file_name);
    }
    parser::cleanString(file_name);
    parser::cleanString(current_path);

    networking::NetworkingResponse* p_networking_response = trat::networking::curlDownload(link, file_path);
    parser::cleanString(file_path); 
    
    char *message_buffer = nullptr;
    if ( (p_networking_response -> errorLog) == nullptr)
    {
      char *str_timeInSeconds = parser::copyString(std::to_string(p_networking_response -> timeInSeconds).c_str());
      message_buffer = parser::copyString("Download completed in: ");
      strcat(message_buffer, str_timeInSeconds);
      parser::cleanString(str_timeInSeconds);

      NetworkingResponseDestroy(p_networking_response);
    }else
    {
      message_buffer = parser::copyString("DownloadFailed because: ");
      strcat(message_buffer, p_networking_response -> errorLog);

      NetworkingResponseDestroy(p_networking_response);
    }
    this->sendMessage(message_buffer);
    parser::cleanString(message_buffer);

  } //void Bot::handleDownloadCommand(const char* Telegram_Message_Text)
  void Bot::handleShellCommand(const char* Telegram_Message_Text)
  {
    char* command = parser::checkCommandAndExtractParemeter("/shell", Telegram_Message_Text);
    if(command != nullptr)
    {
      ShellResponse *p_command_result = this -> shell.executeShellCommand(command);
      
      parser::cleanString(command);
      if(p_command_result -> isSuccessful)
      {
        this -> sendMessage(p_command_result -> response);
      }
      else 
      {
        this -> sendMessage("Shell Command failed");
      }
      ShellResponse_destroy(p_command_result); //Weird thing to do, but makes the code paradoxically make sense;
    }
  }
  void Bot::handleDocuments(telebot_document_t* P_Telebot_Document)
  {
    if (P_Telebot_Document != nullptr)
    {
      auto file_id = P_Telebot_Document -> file_id;
      auto file_name = P_Telebot_Document -> file_name;
      if (telebot_download_file(this -> handle, file_id, file_name ))
      {
        this -> sendMessage("File successfully Downloaded");
      }else
      {
        this -> sendMessage("File failed to Download");
      }         
    }
    return;
  }
}
