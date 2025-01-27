/* trat/include/parser.hpp */
#ifndef TRAT__PARSER_HPP
#define TRAT__PARSER_HPP

#include <cstdint>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <string>
#include <filesystem>

#include <algorithm> // For std::find and std::partition
#ifdef WIN32 
# define PATH_SEPERATOR '\\'
#else
# define PATH_SEPERATOR '/'
#endif
namespace trat
{
  namespace parser 
  {
    char* copyString(const char* String);
    
    void cleanString(char* String);
    /* Both of these functions return -1 if that is not the case, otherwise they return the position of the Char, or the first Char of the sub string */
    int isCharInWord(const char Char, const char* Word);
    int isSubStringPartOfString(const char* Sub_String, const char* String );

    /* Each of these functions requires void cleanString(char* String) after use */
    char* getInvertedString(const char* String);
    
    char* getPrefixFromString(const char* String, const char* Seperator);

    char* getSuffixFromString(const char* String, const char* Seperator);
    
    /* invert the name and get the prefix, reinvert */
    char* getExtension(const char* File_Name);

    char* getFileNameFromLink(char* Link);
    
    typedef struct PrefixSuffix
    {
      char *prefix;
      char *suffix;
    }PrefixSuffix;
    
    PrefixSuffix *breakWordIntoPrefixSuffix(const char* Word, const char SeperatorChar);
    void PrefixSuffixDestroy(PrefixSuffix* P_Prefix_Suffix);

        /* The result of this needs to be freed */ 
    char* checkCommandAndExtractParemeter(const char* Command, const char* Text);
    
    
    char* constructFilePath(const char* Path, const char* File_Name);

  }//namespace parser

} //namespace trat 

#endif //TRAT__PARSER_HPP
