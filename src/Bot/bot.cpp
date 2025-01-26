/* trat/src/Bot/bot.cpp */
#include "../../include/bot.hpp"

namespace trat 
{

  Bot::Bot(const char* Token, const int64_t& Client_Id)
  :token(strdup(Token)),
   clientId(Client_Id),
   shell(Shell(this))
  {
    if (telebot_create(&handle, token) != TELEBOT_ERROR_NONE)
    {
      printf("failed to create bot");
      exit(EXIT_FAILURE);
    }
  
  }
  Bot::~Bot()
  {
    telebot_destroy(handle);  // Pass p_handle directly
    if (token != nullptr)
    {
      free(token);  // Free the allocated memory for the token
      token = nullptr;
    }
  }
  
  bool Bot::downloadFromChat(const char* File_Id, const char* File_Path)
  {
    if(!telebot_download_file(this -> handle, File_Id, File_Path ))
    {
      return false;
    }
    return true;
  }
}  // namespace trat
