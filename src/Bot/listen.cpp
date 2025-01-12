#include "../../include/bot.hpp"

namespace trat
{
    void Bot::listen()
    {
        int offset = -1;
        telebot_update_type_e update_types[] = {TELEBOT_UPDATE_TYPE_MESSAGE};
        telebot_error_e ret;
        
        while (true)
        {
            telebot_update_t* updates = nullptr; // Declare outside the loop
            int count = 0; // Declare outside the loop
            
            ret = telebot_get_updates(handle, offset, 20, 0, update_types, 1, &updates, &count);
            if (ret != TELEBOT_ERROR_NONE)
            {
                std::this_thread::sleep_for(std::chrono::seconds(2));
                continue;
            }

            for (int i = 0; i < count; ++i)
            {
                const auto& update = updates[i];

                if (update.update_type == TELEBOT_UPDATE_TYPE_MESSAGE)
                {
                    const auto& message = update.message;
                    this -> handleDocuments(message.document);
                    if (message.text)
                    {
						    this -> handleTextBasedCommand(message.text, "/pwd", (this -> shell).getCurrentPath());
                            this -> handleDownloadCommand(message.text);
                    }
                }
                // Update the offset to the latest update ID
                offset = updates[i].update_id + 1;
            }

            // Release the memory allocated for updates
            telebot_put_updates(updates, count);
        }
    }
}
