#ifndef SHELL_HPP
#define SHELL_HPP

#include <string>
#include <cstring>
#include <vector>
#include <unordered_set>
#include <algorithm>
#include <iostream>
#include <filesystem>

namespace trat {

    typedef enum {
        TELEGRAM_API_NATURE_SIMPLE,  // Bot handles simple commands directly
        TELEGRAM_API_NATURE_COMPLEX, // Bot parses and executes complex commands
        SHELL_NATURE_SIMPLE,         // Shell handles simple commands (no parameters)
        SHELL_NATURE_HAS_PARAMETER,  // Shell commands with parameters
        SHELL_NATURE_COMPLEX         // Complex shell commands (e.g., raw shell input)
    } CommandType;

    // Structure representing a command
    typedef struct {
        std::string literal;           // The full command as received
        CommandType type;              // The categorized type
        std::vector<std::string> parts; 
    } Command;

    typedef struct {
        bool isSuccessful;             // Indicates success of the operation
        std::string response;          // Response from the shell or bot
    } ShellResponse;

    
    class Bot;
    
    class Shell {
    private:
        Bot* masterBot;

        const std::vector<const char*> tratBotRelatedCommands = {"/echo", "/status"};
        const std::vector<const char*> shellCommands = {"/ls", "/rm", "/cd", "/download", "/upload"};

    public:

        Shell(trat::Bot* MasterBot);

        
        char* getCurrentPath();

        void debugCommand(const Command& command);
        void runCommand(const char* command);
        //ShellResponse execute(const Command& parsedCommand);


        //ShellResponse download(const std::string& url);
    };

} // namespace trat

#endif // SHELL_HPP
