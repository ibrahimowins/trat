/* src/Bot/work.cpp */
#include "../../include/bot.hpp"

namespace trat 
{
  void Bot::work()
  {
    
    if (this -> sendMessage("Hello World!"))
    {
      this -> listen();
    }
  }
}
