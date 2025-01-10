#include "../../include/bot.hpp"


namespace trat
{
    /*A wrapper over 
    telebot_send_message(
            telebot_handler_t handle,
            long long int chat_id,
            const char *text, 
            const char *parse_mode, 
            bool disable_web_page_preview,
            bool disable_notification, 
            int reply_to_message_id, 
            const char *reply_markup  
                )*/
    bool Bot::sendMessage(const char* Message)
    {
        if (Message == nullptr)
        {
            return false;
        }

        if (
            telebot_send_message(handle, clientId, Message, "HTML", true, false,  0, "") 
            != 
            TELEBOT_ERROR_NONE)
        {
            return false;
        }
        return true;
    }

    /*
    telebot_send_photo(
    telebot_handler_t handle, 
    long long int chat_id,
    const char *photo, 
    bool is_file, 
    const char *caption,
    const char *parse_mode,
    bool disable_notification, 
    int reply_to_message_id, 
    const char *reply_markup)
    */
    bool Bot::sendPhoto(const char* FilePath)
    {
        if (FilePath == nullptr)
        {
            return false;
        }
        if(
            telebot_send_photo(handle, clientId, FilePath, true, "", "HTML", false,
                                0, ""
            ) != TELEBOT_ERROR_NONE
        )
        {
            return false;
        }
        return true;
    }
    bool Bot::sendDocument(const char* DocumentPath)
    {
        if (DocumentPath == nullptr)
        {
            return false;
        }
        if(
            telebot_send_document(handle, clientId, DocumentPath, true, "", ""
            , "HTML", false, 0, "") != TELEBOT_ERROR_NONE
        )
        {
            return false;
        }
        return true;
    }
}

