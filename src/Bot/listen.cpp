
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
            telebot_update_t* updates;
            int count;

            // Fetch updates from Telegram
            ret = telebot_get_updates(handle, offset, 20, 0, update_types, 1, &updates, &count);

            if (ret != TELEBOT_ERROR_NONE)
            {
                std::cerr << "Error fetching updates: " << ret << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(2));
                continue;
            }

            std::cout << "Number of updates: " << count << std::endl;

            for (int i = 0; i < count; ++i)
            {
                const auto& update = updates[i];

                if (update.update_type == TELEBOT_UPDATE_TYPE_MESSAGE)
                {
                    const auto& message = update.message;
                    if (message.text)
                    {
                        if (message.document)
                        {
                            const auto& document = message.document;
                            auto fileId = document -> file_id;
                            auto fileName = document -> file_name;
                            std::thread([this, fileId, fileName]() {
                                    this -> downloadFromChat(fileId, fileName);
                                }).detach();

                        }
                        
                        else
                        {
                            if (strstr(message.text, "/pwd"))
                            {
                                std::thread([this]() {
                                    this->sendMessage(this -> p_shell -> getCurrentPath());
                                }).detach();
                            }


                            else if (auto link = extractDownloadLink(message.text))
                            {
                                std::thread([this, link = std::move(link)]() 
                                    {

                                        auto networking_response = trat::curlDownload(link, constructFilePath(link));
                                        char message_buffer[256];
                                        if (networking_response.isSuccessful)
                                        {
                                            snprintf(message_buffer, sizeof(message_buffer), "Download completed in %f",  networking_response.timeInSeconds);   
                                        }
                                        else
                                        {
                                            snprintf(message_buffer, sizeof(message_buffer), "DownloadFailed: %s",  networking_response.errorLog);
                                        }
                                        this->sendMessage(message_buffer);
                                    }).detach();
                            }
                        }
                    }

                // Update the offset to the latest update ID
                offset = updates[i].update_id + 1;
                }
            }
            // Release the memory allocated for updates
            telebot_put_updates(updates, count);
        }
    }
}
