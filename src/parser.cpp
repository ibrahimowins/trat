#include "../include/parser.hpp"

namespace trat
{
    const char* extractDownloadLink(const char* text)
    {
        const char* commandPrefix = "/download ";
        size_t prefixLength = std::strlen(commandPrefix);

        if (std::strncmp(text, commandPrefix, prefixLength) == 0)
        {
            // Extract the link after the prefix
            const char* link = text + prefixLength;
            if (std::strlen(link) > 0)
            {
                return link; // Return the link
            }
        }
        return nullptr; // Return empty if no valid link
    }
        // Extract the file name from a URL or file path
    const char* extractFileNameFromLink(const char* link)
    {
        if (link == nullptr || std::strlen(link) == 0)
        {
            return nullptr; // Return null if the link is invalid
        }

        // Find the last '/' character to get the start of the file name
        const char* lastSlash = std::strrchr(link, '/');
        if (lastSlash != nullptr)
        {
            // Move the pointer past the last '/'
            return lastSlash + 1;
        }

        // If no '/' is found, assume the entire link is the file name
        return link;
    }

    // Extract the file extension from a file name or URL
    const char* extractFileExtensionFromLink(const char* link)
    {
        // Get the file name first
        const char* fileName = extractFileNameFromLink(link);
        if (fileName == nullptr || std::strlen(fileName) == 0)
        {
            return nullptr; // Return null if the file name is invalid
        }

        // Find the last '.' character in the file name
        const char* lastDot = std::strrchr(fileName, '.');
        if (lastDot != nullptr && *(lastDot + 1) != '\0')
        {
            // Return the extension starting after the last '.'
            return lastDot + 1;
        }

        // If no '.' is found, return null (no extension)
        return nullptr;
    }
    const char* constructFilePath(const char* link)
    {
        // Validate the input link
        if (link == nullptr || std::strlen(link) == 0)
        {
            return nullptr; // Return null if the link is invalid
        }

        // Extract the file name from the link
        const char* fileName = extractFileNameFromLink(link);
        if (fileName == nullptr || std::strlen(fileName) == 0)
        {
            return nullptr; // Return null if the file name is invalid
        }

        try
        {
            // Get the current working directory
            std::string currentPath = std::filesystem::current_path().string();

            // Construct the full path by appending the file name
            std::string fullPath = currentPath + "/" + fileName;

            // Allocate memory for the result and copy the full path
            char* result = new char[fullPath.length() + 1];
            std::strcpy(result, fullPath.c_str());
            return result;
        }
        catch (const std::exception& e)
        {
            return nullptr;
        }
    }
}
