#ifndef NETWORKING_HPP
#define NETWORKING_HPP

#include <stdio.h>    // For file operations like fopen, fwrite, fclose, fstat
#include <stdlib.h>   // For general-purpose functions like malloc, free, exit
#include <string.h>   
#include <curl/curl.h> 
#include <sys/stat.h>  // For fstat, which retrieves file information (needed for file size)

#define NETWORKING_BUFFER_SIZE 256
#define FILE_PATH_SEPARATOR '/'  // Adjust this if necessary for different platforms

namespace trat
{
    struct NetworkingResponse
    {
        bool isSuccessful;
        char* errorLog;      // Change to dynamically allocated string
        double timeInSeconds;
    };

    void setFileNameFromFullPath(const char *fullPath, char **filename, char separator);
    void writeCallback(void *Data, size_t SizeDataElement, size_t NumberDataElements, FILE *outputStream);

    NetworkingResponse curlDownload(const char *Url, const char *OutputFilePath);
}

#endif //NETWORKING_HPP
