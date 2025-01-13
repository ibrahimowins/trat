#include "../include/main.hpp"
#include "../include/bot.hpp"


int main() 
{
    using namespace trat;
     
    int64_t chat_id = 6479143326;
    const char* token_buffer =  "7843656436:AAH6rROv5vGc2FiE16lGgD4-6A6OvbruW50";
    Bot bot = Bot(token_buffer, chat_id);
    bot.listen();
    return 0;
}
