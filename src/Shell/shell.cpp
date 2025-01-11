#include "../../include/shell.hpp"

namespace trat
{
    Shell::Shell(trat::Bot *MasterBot):masterBot(MasterBot){};

    char* Shell::getCurrentPath()
    {
        return strdup(std::filesystem::current_path().string().c_str());
    }

}
