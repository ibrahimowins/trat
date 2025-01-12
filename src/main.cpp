#include "../include/main.hpp"

#define TELEGRAM_BOT_API_URL "https://api.telegram.org/bot"


int main() {
 // Replace with your bot token
    using namespace trat;

    int64_t chat_id = std::atoll(std::getenv("6479143326"));                // Replace with your chat ID
    const char* token_buffer =  std::getenv("7843656436:AAH6rROv5vGc2FiE16lGgD4-6A6OvbruW50");
    Bot bot = Bot(token_buffer, chat_id);

    bot.listen();
    return 0;
}
