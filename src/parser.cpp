#include "../include/parser.hpp"
#include <string>
#include <filesystem>
namespace trat
{
  const char* extractDownloadLink(const char* Text)
  {
    const char* command_prefix = "/download ";
    size_t prefix_length = std::strlen(command_prefix);

    if (std::strncmp(Text, command_prefix, prefix_length) == 0)
    {
      // Extract the link after the prefix
      const char* link = Text + prefix_length;
      if (std::strlen(link) > 0)
      {
        return link; // Return the link
      }
    }
    return nullptr; // Return empty if no valid link
  }
  const char* extractShellCommand(const char* Text)
  {
    const char* command_prefix = "/shell";
    size_t prefix_length = std::strlen(command_prefix);
    if (std::strncmp(Text, command_prefix, prefix_length) == 0)
    {
      const char* command = Text + prefix_length;
      if(std::strlen(command) > 0)
      {
        return command;
      }

    }
    return nullptr;
  }
  // Extract the file name from a URL or file path
  const char* extractFileNameFromLink(const char* Link)
  {
    if (Link == nullptr || std::strlen(Link) == 0)
    {
      return nullptr; // Return null if the link is invalid
    }

    // Find the last '/' character to get the start of the file name
    const char* last_slash = std::strrchr(Link, '/');
    if (last_slash != nullptr)
    {
      // Move the pointer past the last '/'
      return last_slash + 1;
    }

    // If no '/' is found, assume the entire link is the file name
    return Link;
  }

  // Extract the file extension from a file name or URL
  const char* extractFileExtensionFromLink(const char* Link)
  {
    // Get the file name first
    const char* file_name = extractFileNameFromLink(Link);
    if (file_name == nullptr || std::strlen(file_name) == 0)
    {
      return nullptr; // Return null if the file name is invalid
    }

    // Find the last '.' character in the file name
    const char* last_dot = std::strrchr(file_name, '.');
    if (last_dot != nullptr && *(last_dot + 1) != '\0')
    {
      // Return the extension starting after the last '.'
      return last_dot + 1;
    }

    // If no '.' is found, return null (no extension)
    return nullptr;
  }
  const char* constructFilePath(const char* Link)
  {
    // Validate the input link
    if (Link == nullptr || std::strlen(Link) == 0)
    {
      return nullptr; // Return null if the link is invalid
    }

    // Extract the file name from the link
    const char* file_name = extractFileNameFromLink(Link);
    if (file_name == nullptr || std::strlen(file_name) == 0)
    {
      return nullptr; // Return null if the file name is invalid
    }

    try
    {
      // Get the current working directory
      std::string current_path = std::filesystem::current_path().string();

      // Construct the full path by appending the file name
      std::string full_path = current_path + "/" + file_name;

      // Allocate memory for the result and copy the full path
      char* result = new char[full_path.length() + 1];
      std::strcpy(result, full_path.c_str());
      return result;
    }
    catch (const std::exception& e)
    {
      return nullptr;
    }
  }
}
