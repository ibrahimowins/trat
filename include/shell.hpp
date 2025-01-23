/* trat/include/shell.hpp */
#ifndef SHELL_HPP
#define SHELL_HPP
#include <boost/process.hpp>
#include <sstream>
#include <filesystem>
#include <cstdlib>

#include <cstring>
#include <iostream>
#include "parser.hpp"

namespace trat 
{
  typedef struct 
  {
    bool isSuccessful;             // Indicates success of the operation
    char *response;          // Response from the shell or bot
  }ShellResponse;
  /* Tricky function, ShellResponse should always be declared in the stack, but its memeber response must be freed, so use &,
   * The thing is this has actually got ridden of the weird must be UTF-8 bug */
  
  void ShellResponse_destroy(ShellResponse *P_Shell_Response);
  
  class Bot; 
  class Shell 
  {
    private:
    Bot* masterBot;
    
    public:
    Shell(trat::Bot* MasterBot);  
     
    /* This must be cleaned */
    char* getCurrentPath();
        
    bool executeShellCommandWithoutResponse(const char* Command);
        
    /* Its reference must be passed to ShellResponse_destroy(ShellResponse *P_Shell_Response) to clean up */
    ShellResponse executeShellCommand(const char* Command); 
    bool isExecutable(const char* File_Name);
  }; //class Shell
} // namespace trat
#endif // SHELL_HPP
