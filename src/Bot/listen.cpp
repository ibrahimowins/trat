/* trat/src/Bot/listen.cpp */
#include "../../include/bot.hpp"
#include <chrono>
#include <thread>

namespace trat
{
  void Bot::listen()
  {
    int offset = -1;
    telebot_update_type_e update_types[] = {TELEBOT_UPDATE_TYPE_MESSAGE};
    telebot_error_e ret;    
    while (true)
    {
      telebot_update_t* updates = nullptr; // Dynamic array of updates 
      int count = 0; 
      ret = telebot_get_updates(handle, offset, 20, 0, update_types, 1, &updates, &count);
      if (ret != TELEBOT_ERROR_NONE)
      {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        continue;
      }
      for (int i = 0; i < count; ++i)
      {
        const auto& update = updates[i];
        if (update.update_type == TELEBOT_UPDATE_TYPE_MESSAGE)
        {
          const auto& message = update.message;
          auto message_text = message.text;
          auto message_document = message.document;
          //auto message_photo = message.photos;          
          if (this -> checkIfCommand(message_text)) 
          // If the message has text in it, invoke those text related methods.
          {
            this -> handleShellCommand(message_text);
            this -> handleDownloadCommand(message_text);
          }
          else 
          {
            /* This runs on a detached thread, because as soon as the program realized that this is not command, it must fetch next 
             * This ensures  that the client recieves the reponse, and the program has already recieved an update */ 
            std::thread
              ([this]()
                {
                  this -> sendMessage("This is not a valid command");
                }
              ).detach();
          }
          if (message_document)
          {
            std::thread(
                [this, message_document]()
                {
                  this -> handleDocuments(message_document);
                }
                ).detach();  
          }
          offset = updates[i].update_id + 1;
          std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
        telebot_put_updates(updates, count);
      }
    }
  }
}
