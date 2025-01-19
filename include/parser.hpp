/* trat/include/parser.hpp */
#ifndef TRAT__PARSER_HPP
#define TRAT__PARSER_HPP

#include <cstring>

namespace trat
{
  namespace parser 
  {
    typedef struct PrefixSuffix
    {
      char *prefix;
      char *suffix;
    }PrefixSuffix;

    /* Use in tendem with its destroyer */
    PrefixSuffix* breakDownWord(const char* Word, const char* Seperator);
    void PrefixSuffix_destroy(PrefixSuffix *P_PrefixSuffix);

    /* The result of this needs to be freed */ 
    char* checkCommandAndExtractParemeter(const char* Command, const char* Text);
    
    const char* extractFileNameFromLink(const char* Link);
    const char* extractFileExtensionFromLink(const char* Link);
    const char* constructFilePath(const char* Link);  

  }
}
#endif //TRAT__PARSER_HPP
