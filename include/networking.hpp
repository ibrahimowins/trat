/* trat/include/networking.hpp */
#ifndef NETWORKING_HPP
#define NETWORKING_HPP

#include <stdio.h>    
#include <stdlib.h>   
#include <string.h>   
#include <curl/curl.h> 
#include <sys/stat.h>  
#include "parser.hpp"
#define NETWORKING_BUFFER_SIZE 256
#define FILE_PATH_SEPARATOR '/'  

namespace trat
{
  namespace networking
  {
    struct NetworkingResponse
    {
        char* errorLog;     
        double timeInSeconds;
    };

    bool checkLinkValidity(const char* Link);
    void setFileNameFromFullPath(const char *Full_Path, char **File_Name, char Separator);
    void writeCallback(void *Data, size_t Size_Data_Element, size_t Number_Data_Elements, FILE *Output_Stream);

    char* getFileNameFromLink(char* Link);
    
    NetworkingResponse* curlDownload(const char *Url, const char *Output_File_Path);
    void NetworkingResponseDestroy(NetworkingResponse *P_Networking_Response);
  }
}

#endif //NETWORKING_HPP
