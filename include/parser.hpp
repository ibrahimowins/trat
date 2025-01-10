#ifndef TRAT__PARSER_HPP
#define TRAT__PARSER_HPP

#include <cstring>
#include <string>
#include <filesystem> //Used to construct the filePath;
namespace trat
{
    const char* extractDownloadLink(const char* text);
    const char* extractFileNameFromLink(const char* link);
    const char* extractFileExtensionFromLink(const char* link);
    const char* constructFilePath(const char* link);
}
#endif //TRAT__PARSER_HPP