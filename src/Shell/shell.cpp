#include "../../include/shell.hpp"
#include <filesystem>
#include <cstdlib>
namespace trat
{
    Shell::Shell(trat::Bot *MasterBot):masterBot(MasterBot){}; //Constructor

    char* Shell::getCurrentPath()
    {
        return strdup(std::filesystem::current_path().string().c_str());
    }
    bool Shell::executeShellCommandWithoutResponse(const char* Command)
    {
      if (std::system(Command))
      {
        return true; 
      }
      return false;
    }

}
