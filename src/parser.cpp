/* trat/src/parser.cpp */
#include "../include/parser.hpp"
#include <boost/process/io.hpp>
#include <cstdlib>
#include <cstring>

namespace trat
{
  namespace parser
  {
    #if WIN32 
    const char PATH_SEPERATOR = '\\';
    #else
    const char PATH_SEPERATOR = '/';
    #endif
    
    char* copyString(const char* String)
    {
      if (!String)  {return nullptr;}
      
      size_t string_size = std::strlen(String);
      char* copied_string = (char*)malloc(sizeof(char) * (string_size + 1) ); 
      
      if (!copied_string) {return nullptr;}

      memcpy(copied_string, String, string_size); 
      return copied_string;
    }
    
    void cleanString(char* String)
    {
      if (String != nullptr)
      {
        free(String);
        String = nullptr;
      }
    }
    
    char* getInvertedString(const char* String)
    {
      if(!String) { return nullptr;}
      size_t string_size = strlen(String);
      char* inverted_string_result = (char*)malloc(sizeof(char) * (string_size + 1));
      if(!inverted_string_result) { return nullptr; }
      
      inverted_string_result[string_size] = '\0';
      char temperory_char;

      for(size_t i = 0; i < string_size; ++i)
      {
        temperory_char = String[i];
        inverted_string_result[string_size - 1 - i] = temperory_char;
      }
      return inverted_string_result;
    }
  
    char* getPrefixFromString(const char* String, const char SeperatorChar)
    {
      if (!String){ return nullptr; }

      size_t seperator_position = 0;
      size_t string_size = std::strlen(String);
  
      for (size_t i = 0; i < string_size; ++i)
      {
        if(String[i] == SeperatorChar)
        {
          seperator_position = i;
          break;
        }
      }
      if (seperator_position == 0)
      {
        return nullptr;
      }
      char* prefix = (char*)malloc(sizeof(char) * seperator_position);
      prefix[seperator_position] = '\0';
      for (size_t i = 0; i < seperator_position; ++i)
      {
        prefix[i] = String[i];
      }
      return prefix;
    }
    char* getSuffixFromString(const char* String, const char SeperatorChar)
    {
      char* invertedString = getInvertedString(String);
      char* prefix = getPrefixFromString(String, SeperatorChar);
      cleanString(invertedString);
      char* suffix = getInvertedString(prefix);
      cleanString(prefix);
      return suffix;
    }
    char* constructFilePath(const char* Path, const char* File_Name)
    {
      if ( (Path == nullptr) || (File_Name == nullptr) )
      {
        return nullptr;
      }
      
      char* path = parser::copyString(Path);
      if (!path)  { return nullptr; }
      
      char* file_name = parser::copyString(File_Name);
      if (!file_name) 
      {
        parser::cleanString(path);
        return nullptr;
      }
      size_t path_size = std::strlen(path);

      char* path_with_seperator = (char*)malloc(sizeof(char) * (path_size) + 1 + 1);
      path_with_seperator[path_size + 1] = '\0';
      path_with_seperator[path_size] = PATH_SEPERATOR;
      for (size_t i = 0; i < path_size; i++)
      {
        path_with_seperator[i] = path[i];
      }
      strcat(path_with_seperator, file_name);
      free(file_name);
      file_name = nullptr;
      free(path);
      path = nullptr;
      return path_with_seperator;
    }
    char* getFileNameFromLink(char* Link)
    {
      char* inverted_link = getInvertedString(Link);
      if(!inverted_link)
      {
        return nullptr;
      }
      char* inverted_file_name = getPrefixFromString(inverted_link, '/');
      free(inverted_link);
      inverted_link = nullptr;
      if(!inverted_file_name)
      {
        return nullptr;
      }
      char* result = getInvertedString(inverted_file_name);
      parser::cleanString(inverted_file_name);
      
      return result;
    }
    
    PrefixSuffix *breakWordIntoPrefixSuffix(const char* Word, const char SeperatorChar)
    {
      if(!Word) {return nullptr;}
      PrefixSuffix *p_result = (PrefixSuffix*)malloc(sizeof(PrefixSuffix));
      if (!p_result)  { return nullptr; }
      p_result -> prefix = getPrefixFromString(Word, SeperatorChar);
      if(!(p_result -> prefix)) 
      {
        free(p_result);
        p_result = nullptr;
        return nullptr;
      }
      p_result -> suffix = getSuffixFromString(Word, SeperatorChar);
      if(!(p_result -> suffix)) 
      {
        parser::cleanString(p_result -> prefix);
        free(p_result);
        p_result = nullptr;
        return nullptr;
      }
      return p_result;
    }
    void PrefixSuffixDestroy(PrefixSuffix* P_Prefix_Suffix)
    {
      if(!P_Prefix_Suffix)  {return;}
      if( !(P_Prefix_Suffix -> prefix) && !(P_Prefix_Suffix -> suffix) )
      {
        return;
      }
      if( !(P_Prefix_Suffix -> prefix)  )
      {
        if(!(P_Prefix_Suffix -> suffix)){ return; }
        parser::cleanString(P_Prefix_Suffix -> suffix);
      }
      if( !(P_Prefix_Suffix -> suffix)  )
      {
        if(!(P_Prefix_Suffix -> prefix)){ return; }
        parser::cleanString(P_Prefix_Suffix -> prefix);
      }
    }
    
    char* checkCommandAndExtractParemeter(const char* Command, const char* Text)
    {
      if ( (!Command)  || (!Text) ){  return nullptr; }
      
      size_t command_length = std::strlen(Command);
      size_t text_length = std::strlen(Text);
      if (text_length <= command_length ) { return nullptr; }
      size_t parameter_length = text_length - command_length;

      char* extracted_command = getPrefixFromString(Text, ' ');
      if (!extracted_command) { return nullptr; }
      
      if ( strcmp(Command, extracted_command) != 0)
      {
        cleanString(extracted_command);
        return nullptr;
      }
      char* parameter = (char*)malloc(sizeof(char) * (parameter_length + 1 ));
      parameter[parameter_length] = '\0';
      for (size_t i = command_length + 1; i < text_length; ++i)
      {
        parameter[i - (command_length + 1)] = Text[i];
      }
      cleanString(extracted_command);
      return parameter;
    }
   
    const char* extractFileNameFromLink(const char* Link)
    {
      if (Link == nullptr || std::strlen(Link) == 0)
        return nullptr;

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

  char* getFileExtensionFromName(const char* File_Name)
  {
    if(!File_Name)  { return nullptr;}
    char* extension = parser::getSuffixFromString(File_Name, '.');
    if (!extension) { return nullptr;}
    return extension;
  }
  
} //namespace  parser
  
