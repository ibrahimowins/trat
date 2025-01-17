/* trat/src/Bot/handling.cpp */
#include <cstring>
#include "../../include/bot.hpp"
#include "../../include/networking.hpp" //For curlDownload()
#include "../../include/parser.hpp" // For extractDownloadLink()

namespace trat
{
  bool Bot::checkIfCommand(const char* Message) 
  {
    if(Message == nullptr)
    {
      return false;
    }
    parser::PrefixSuffix *p_prefix_and_suffix = parser::breakDownWord(Message, " ");
    if (p_prefix_and_suffix == nullptr)
    {
      return false;
    }
    if (strcmp(p_prefix_and_suffix -> prefix, "") == 0)
    {
      if (p_prefix_and_suffix != nullptr)
      {
        parser::PrefixSuffix_destroy(p_prefix_and_suffix);
      }
      return false;
    }else
    {
      for(auto i = 0 ; i < NUMBER_RECOGNIZED_COMMANDS; ++i)
      {
        if ( strcmp(p_prefix_and_suffix -> prefix, (this -> commands)[i]) == 0 )
        {
          if (p_prefix_and_suffix != nullptr)
          {
            parser::PrefixSuffix_destroy(p_prefix_and_suffix);
          }
          return true;
        }
      }
      if (p_prefix_and_suffix != nullptr)
      {
        parser::PrefixSuffix_destroy(p_prefix_and_suffix);
      }
      return false;
    }
  }
  void Bot::handleTextBasedCommand(const char* Telegram_Message_Text, const char* Command,  char* Shell_Function_Callback_Result)
  {
    if ((Telegram_Message_Text != nullptr)&&(Command != nullptr)&&(Shell_Function_Callback_Result != nullptr))
    {
      if (strstr(Telegram_Message_Text, Command))
      {
        /* Aliasing  the pointer */
        char* function_result = Shell_Function_Callback_Result;
        this->sendMessage(function_result);
        free(function_result);
        function_result = nullptr;
      }
    }
  }
  void Bot::handleDownloadCommand(const char* Telegram_Message_Text)
  {
    char* link = parser::checkCommandAndExtractParemeter("/download", Telegram_Message_Text);
    if (link != nullptr)
    {
      networking::NetworkingResponse networking_response = trat::networking::curlDownload(link, parser::constructFilePath(link));
      free(link);
      link = nullptr;
      char message_buffer[256];
      if (networking_response.isSuccessful)
      {
        snprintf(message_buffer, sizeof(message_buffer), "Download completed in %f",  networking_response.timeInSeconds);   
      }else
      {
        snprintf(message_buffer, sizeof(message_buffer), "DownloadFailed: %s",  networking_response.errorLog);
      }
      this->sendMessage(message_buffer);
    }
  }
  void Bot::handleShellCommand(const char* Telegram_Message_Text)
  {
    char* command = parser::checkCommandAndExtractParemeter("/shell", Telegram_Message_Text);
    if(command != nullptr)
    {
      auto command_result = this -> shell.executeShellCommand(command);
      free(command);
      command = nullptr;
      if(command_result.isSuccessful)
      {
        this -> sendMessage(command_result.response);
      }
      else 
      {
        this -> sendMessage("Shell Command failed");
      }
      ShellResponse_destroy(&command_result); //Weird thing to do, but makes the code paradoxically make sense;
    }
  }
  void Bot::handleDocuments(telebot_document_t* P_Telebot_Document)
  {
    if (P_Telebot_Document != nullptr)
    {
      auto fileId = P_Telebot_Document -> file_id;
      auto fileName = P_Telebot_Document -> file_name;
      if (telebot_download_file(this -> handle, fileId, fileName ))
      {
        this -> sendMessage("File successfully Downloaded");
      }else
      {
        this -> sendMessage("File failed to Download");
      }         
    }
  }
}
