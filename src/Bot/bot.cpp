#include "../../include/bot.hpp"
namespace trat 
{

    Bot::Bot(const char* Token, const int64_t& ClientId)
    {
        token = strdup(Token);  // Duplicate the string to manage its lifetime
        clientId = ClientId;

        try
        {
            if (telebot_create(&handle, token) != TELEBOT_ERROR_NONE)  // Pass address of p_handle
            {
                throw std::runtime_error("Failed to create Telebot handler.");
            }
            p_shell = new Shell(this);
        }
        catch (const std::exception& e)
        {
            std::cerr << "Exception in Bot constructor: " << e.what() << std::endl;
            throw;  // Re-throw the exception after logging
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
        if (p_shell != nullptr)
        {
            delete p_shell;
            p_shell = nullptr;
        }
    }
    bool Bot::downloadFromChat(const char* FileId, const char* FilePath)
    {
        if(!telebot_download_file(this -> handle, FileId, FilePath ))
        {
            return false;
        }
        return true;
    }

}  // namespace trat
