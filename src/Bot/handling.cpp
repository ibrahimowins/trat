#include <cstring>
#include "../../include/bot.hpp"
#include "../../include/networking.hpp" //For curlDownload()
#include "../../include/parser.hpp" // For extractDownloadLink()

namespace trat
{
  bool Bot::checkIfCommand(const char* Message) 
  {
    // Extract the prefix (substring before the first space)
    const char* space_pos = std::strchr(Message, ' ');
    size_t prefix_len = (space_pos != nullptr) ? (space_pos - Message) : std::strlen(Message);

    // Check if the extracted prefix matches any command in the array
    for (unsigned int i = 0; i < this -> numberCommands; ++i) 
    {
      auto command = (this -> commands)[i];
      if (std::strncmp(Message, command, prefix_len) == 0 && std::strlen(command) == prefix_len) 
      {
        return true;  // Match found
      }
    }
    return false;  // No match
  }
  void Bot::handleTextBasedCommand(const char* Telegram_Message_Text, const char* Command,  char* Shell_Function_Callback_Result)
  {
    if (strstr(Telegram_Message_Text, Command))
    {
      this->sendMessage(Shell_Function_Callback_Result);
    }

  }
  void Bot::handleDownloadCommand(const char* Telegram_Message_Text)
  {
    if (auto link = extractDownloadLink(Telegram_Message_Text))
    {
      auto networking_response = trat::curlDownload(link, constructFilePath(link));
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
    if (auto command = extractShellCommand(Telegram_Message_Text))
    {
      auto command_result = this -> shell.executeShellCommand(command);
      if(command_result.isSuccessful)
      {
        this -> sendMessage(command_result.response);
      }
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
