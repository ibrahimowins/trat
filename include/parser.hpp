/* trat/include/parser.hpp */
#ifndef TRAT__PARSER_HPP
#define TRAT__PARSER_HPP

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <string>
#include <filesystem>


namespace trat
{
  namespace parser 
  {
    char* copyString(const char* String);
    
    void cleanString(char* String);

    /* Each of these functions requires void cleanString(char* String) after use */
    char* getInvertedString(const char* String);
    
    char* getPrefixFromString(const char* String, const char SeperatorChar);
    char* getSuffixFromString(const char* String, const char SeperatorChar);

    /* invert the name and get the prefix, reinvert */
    char* getExtension(const char* File_Name);

    char* getFileNameFromLink(char* Link);
    
    char* constructFilePath(const char* Path, const char* File_Name, const char Path_Seperator);
    
    typedef struct PrefixSuffix
    {
      char *prefix;
      char *suffix;
    }PrefixSuffix;
    
    PrefixSuffix *breakWordIntoPrefixSuffix(const char* Word, const char SeperatorChar);
    void PrefixSuffixDestroy(PrefixSuffix* P_Prefix_Suffix);

        /* The result of this needs to be freed */ 
    char* checkCommandAndExtractParemeter(const char* Command, const char* Text);
    
    const char* extractFileNameFromLink(const char* Link);
    const char* extractFileExtensionFromLink(const char* Link);
    
    char* constructFilePath(const char* Path, const char* File_Name);
    char* getFileExtensionFromName(const char* File_Name);  

  }//namespace parser

} //namespace trat 

#endif //TRAT__PARSER_HPP
