/* trat/src/Bot/sending.cpp */
#include "../../include/bot.hpp"

namespace trat
{
  bool Bot::sendMessage(const char* Message)
  {
    if (Message == nullptr)
    {
      return false;
    }

    if (telebot_send_message(handle, clientId, Message, "HTML", true, false,  0, "") 
        != TELEBOT_ERROR_NONE)
    {
      return false;
    }
    return true;
  }

  bool Bot::sendPhoto(const char* File_Path)
  {
    if (File_Path == nullptr)
    {
      return false;
    }
    if( telebot_send_photo(handle, clientId, File_Path, true, "", "HTML", false, 0, "") 
        != TELEBOT_ERROR_NONE)
    {
      return false;
    }
    return true;
  }

  bool Bot::sendDocument(const char* Document_Path)
  {
    if (!Document_Path)
    {
      return false;
    }
    if(
      telebot_send_document(handle, clientId, Document_Path, true, "", ""
      , "HTML", false, 0, "") != TELEBOT_ERROR_NONE
    )
    {
      return false;
    }
    return true;
  }
}
