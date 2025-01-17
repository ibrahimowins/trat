/* trat/include/emailer.hpp */
#ifndef EMAILER_HPP
#define EMAILER_HPP

#include "bot.hpp"


#define EMAIL_ADRESS_BUFFER_SIZE 128
#define EMAIL_PASSWORD_BUFFER_SIZE 64

namespace trat
{
    
  class Emailer
  {
    private:
    char emailAdress[EMAIL_ADRESS_BUFFER_SIZE];
    char password[EMAIL_PASSWORD_BUFFER_SIZE];
    const Bot* botMaster;
    public:
    Emailer();
    ~Emailer();

    bool sendEmail();
    };
} //namspace trat

#endif //EMAILER_HPP
