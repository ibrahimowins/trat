#ifndef TRAT__PARSER_HPP
#define TRAT__PARSER_HPP

#include <cstring>
namespace trat
{
    const char* extractDownloadLink(const char* Text);
    const char* extractFileNameFromLink(const char* Link);
    const char* extractFileExtensionFromLink(const char* Link);
    const char* constructFilePath(const char* Link);
    
    const char* extractShellCommand(const char* Text);
}
#endif //TRAT__PARSER_HPP
