/* trat/src/Bot/listen.cpp */
#include "../../include/bot.hpp"
#include <chrono>
#include <cstring>
#include <thread>
#include <future>
#include <vector>
namespace trat
{
  void Bot::listen()
  {
    int offset = -1;
    telebot_update_type_e update_types[] = {TELEBOT_UPDATE_TYPE_MESSAGE};
    telebot_error_e ret;    
    std::vector<std::future<void>> futures = {};
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
            futures.push_back(std::move(future));
          }
          if ( (message_document) && (!message_text) )
          {
            auto future = std::async(std::launch::async, [this, message_document]()
            {
              this -> sendMessage("Downloading document");
              this -> handleDocuments(message_document);
            });
            futures.push_back(std::move(future));
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
              parser::cleanString(precieved_command);
              
              futures.push_back(std::move(future));
            }
            parser::cleanString(precieved_command);
          }
          
          offset = update.update_id + 1;
          std::this_thread::sleep_for(std::chrono::seconds(1));
        }
      }    
      auto it = futures.begin();
      while (it != futures.end()) 
      {
        if (it->wait_for(std::chrono::seconds(0)) == std::future_status::ready) 
        {
          try { it->get();} 
          catch (const std::exception& e) { std::cerr << "Exception in asynchronous task: " << e.what() << '\n'; }
        it = futures.erase(it);
        } 
        else{ ++it; }
      }
      telebot_put_updates(updates, count);
    }
  }
}
