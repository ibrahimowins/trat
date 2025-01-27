/* trat/src/parser.cpp */
#include "../include/parser.hpp"
#include <boost/process/io.hpp>
#include <cstdlib>
#include <cstring>

namespace trat
{
  namespace parser
  {

    char* copyString(const char* String)
    {
      if (!String)  {return nullptr;}
      
      size_t string_size = std::strlen(String);
      char* copied_string = (char*)malloc(sizeof(char) * (string_size + 1) ); 
      
      if (!copied_string) {return nullptr;}

      memcpy(copied_string, String, string_size); 
      copied_string[string_size] = '\0'; // Ensure null-termination
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
   
    int isCharInWord(const char Char, const char* Word) 
    {
      if (!Word || Char == '\0') { return -1; }

      for (size_t i = 0; Word[i] != '\0'; ++i) 
      {
        if (Word[i] == Char) { return (int)i; }
      }
      return -1;
    }

    int isSubStringPartOfString(const char* Sub_String, const char* String) 
    {
      if (!Sub_String || !String) { return -1; }

      size_t sub_string_size = strlen(Sub_String);
      size_t string_size = strlen(String);

      if (sub_string_size > string_size) { return -1; }

      for (size_t i = 0; i <= string_size - sub_string_size; ++i) 
      {
        if (strncmp(String + i, Sub_String, sub_string_size) == 0) 
        {
          return (int)i;
        }
      }
      return -1;
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
  
    char* getPrefixFromString(const char* String, const char* Seperator)
    {
      if ((!String) || (!Seperator)){ return nullptr; }
      int seperator_position = isSubStringPartOfString(Seperator, String);
      if(seperator_position == -1)
      {
        return nullptr;
      }
      char* prefix = (char*)malloc(sizeof(char) * (seperator_position + 1));
      if (!prefix) { return nullptr; }
      strncpy(prefix, String, seperator_position);
      prefix[seperator_position] = '\0'; // Ensure null-termination
      return prefix;
    }

    char* getSuffixFromString(const char* String, const char* Seperator)
    {
      if((!String) || (!Seperator)) { return nullptr; }
      int seperator_length = strlen(Seperator);
      int string_length = strlen(String);
      if(seperator_length > string_length ) { return nullptr; }
      int suffix_position = isSubStringPartOfString(Seperator, String) + seperator_length;
      int suffix_length = string_length - suffix_position;
      char* suffix = (char*)malloc(sizeof(char) * (suffix_length + 1));
      if(!suffix) {return nullptr;}
      strncpy(suffix, String + suffix_position, suffix_length);
      suffix[suffix_length] = '\0'; 
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

      char* path_with_seperator = (char*)malloc(sizeof(char) * (path_size + 1 + std::strlen(file_name) + 1));
      if (!path_with_seperator) 
      {
        parser::cleanString(path);
        parser::cleanString(file_name);
        return nullptr;
      }
      strcpy(path_with_seperator, path);
      path_with_seperator[path_size] = PATH_SEPERATOR;
      path_with_seperator[path_size + 1] = '\0';
      strcat(path_with_seperator, file_name);
      
      parser::cleanString(file_name);
      parser::cleanString(path);
      return path_with_seperator;
    }

    char* getFileNameFromLink(char* Link)
    {
      char* inverted_link = getInvertedString(Link);
      if(!inverted_link)
      {
        return nullptr;
      }
      char* inverted_file_name = getPrefixFromString(inverted_link, "/");
      cleanString(inverted_link);
      
      if(!inverted_file_name)
      {
        return nullptr;
      }
      char* result = getInvertedString(inverted_file_name);
      parser::cleanString(inverted_file_name);
      
      return result;
    }
    
    PrefixSuffix *breakWordIntoPrefixSuffix(const char* Word, const char* Root)
    {
      if((!Word) || (!Root)) {return nullptr;}
      PrefixSuffix *p_result = (PrefixSuffix*)malloc(sizeof(PrefixSuffix));
      if (!p_result)  { return nullptr; }
      
      p_result->prefix = getPrefixFromString(Word, Root);
      p_result->suffix = getSuffixFromString(Word, Root);
      
      if(!(p_result->prefix) || !(p_result->suffix)) 
      {
        if (p_result->prefix) { parser::cleanString(p_result->prefix); }
        if (p_result->suffix) { parser::cleanString(p_result->suffix); }
        free(p_result);
        return nullptr;
      }
      return p_result;
    }

    void PrefixSuffixDestroy(PrefixSuffix* P_Prefix_Suffix)
    {
      if(!P_Prefix_Suffix)  {return;}
      if(P_Prefix_Suffix->prefix) 
      {
        parser::cleanString(P_Prefix_Suffix->prefix);
      }
      if(P_Prefix_Suffix->suffix) 
      {
        parser::cleanString(P_Prefix_Suffix->suffix);
      }
      free(P_Prefix_Suffix);
    }
    
    char* checkCommandAndExtractParemeter(const char* Command, const char* Text)
    {
      if ( (!Command)  || (!Text) ){  return nullptr; }
      
      size_t command_length = std::strlen(Command);
      size_t text_length = std::strlen(Text);
      if (text_length <= command_length ) { return nullptr; }
      size_t parameter_length = text_length - command_length;

      char* extracted_command = getPrefixFromString(Text, " " );
      if (!extracted_command) { return nullptr; }
      
      if ( strcmp(Command, extracted_command) != 0)
      {
        cleanString(extracted_command);
        return nullptr;
      }
      char* parameter = (char*)malloc(sizeof(char) * (parameter_length + 1 ));
      if (!parameter) 
      {
        cleanString(extracted_command);
        return nullptr;
      }
      strncpy(parameter, Text + command_length + 1, parameter_length);
      parameter[parameter_length] = '\0';
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

 }
} //namespace  parser
