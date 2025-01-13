#ifndef EMAILER_HPP
#define EMAILER_HPP

#include "bot.hpp"



namespace trat
{
    
    class Emailer
    {
        private:
            std::string workingPath;
            std::string emailAdress;
            std::string password;
            Bot* botMaster;
        public:
            Emailer();
            ~Emailer();
            
            bool sendEmail();

    };
}

#endif EMAILER_HPP
