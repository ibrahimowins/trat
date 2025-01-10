#include "../include/networking.hpp"

namespace trat
{
    void setFileNameFromFullPath(const char *fullPath, char **filename, char separator) 
    {
        // Find the last occurrence of the path separator
        const char *separatorPos = strrchr(fullPath, separator);

        if (separatorPos) 
        {
            // Allocate memory for the filename (everything after the separator)
            *filename = strdup(separatorPos + 1);  // Ensure strdup is successful
            if (*filename == nullptr)
            {
                // Memory allocation failed
                *filename = nullptr;
            }
        } 
        else 
        {
            // If no separator found, treat the full path as the filename
            *filename = strdup(fullPath);
            if (*filename == nullptr)
            {
                // Memory allocation failed
                *filename = nullptr;
            }
        }
    }

    // Callback function for writing data
    void writeCallback(void *Data, size_t SizeDataElement, size_t NumberDataElements, FILE *OutputStream)
    {
        fwrite(Data, SizeDataElement, NumberDataElements, OutputStream);
    }

    // Function to handle downloading a file using libcurl
    NetworkingResponse curlDownload(const char* Url, const char* OutputFilePath)
{
    CURL* curl;
    CURLcode res;
    FILE* fp;
    NetworkingResponse response = {false, nullptr, 0.0};  // Initialize with default values
    clock_t start, end;

    curl = curl_easy_init();
    if (!curl)
    {
        response.errorLog = strdup("Failed to initialize curl");
        return response;
    }

    char* filename;
    setFileNameFromFullPath(OutputFilePath, &filename, FILE_PATH_SEPARATOR);

    if (!filename)
    {
        response.errorLog = strdup("Failed to allocate memory for filename");
        return response;
    }

    fp = fopen(filename, "wb");
    if (!fp)
    {
        curl_easy_cleanup(curl);
        response.errorLog = strdup("Failed to open file");
        free(filename);
        return response;
    }

    curl_easy_setopt(curl, CURLOPT_URL, Url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);

    start = clock();

    res = curl_easy_perform(curl);

    end = clock();

    response.timeInSeconds = ((double)(end - start)) / CLOCKS_PER_SEC;

    if (res != CURLE_OK)
    {
        response.errorLog = strdup(curl_easy_strerror(res));
        response.isSuccessful = false;
    }
    else
    {
        response.errorLog = strdup("");  // Empty string indicates no error
        response.isSuccessful = true;
    }

    fclose(fp);
    curl_easy_cleanup(curl);
    free(filename);

    return response;
}


  NetworkingResponse curlUpload(const char* FilePath, const char* TargetUrl)
{
    CURL* curl;
    CURLcode res;
    FILE* fp;
    struct stat file_info;
    NetworkingResponse response = {false, nullptr, 0.0};  // Initialize with default values
    clock_t start, end;

    if (stat(FilePath, &file_info))
    {
        response.errorLog = strdup("Failed to stat file");
        return response;
    }

    curl = curl_easy_init();
    if (!curl)
    {
        response.errorLog = strdup("Failed to initialize curl");
        return response;
    }

    fp = fopen(FilePath, "rb");
    if (!fp)
    {
        curl_easy_cleanup(curl);
        response.errorLog = strdup("Failed to open file");
        return response;
    }

    curl_easy_setopt(curl, CURLOPT_URL, TargetUrl);
    curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
    curl_easy_setopt(curl, CURLOPT_READDATA, fp);
    curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE, (curl_off_t)file_info.st_size);

    start = clock();

    res = curl_easy_perform(curl);

    end = clock();

    response.timeInSeconds = ((double)(end - start)) / CLOCKS_PER_SEC;

    if (res != CURLE_OK)
    {
        response.errorLog = strdup(curl_easy_strerror(res));
        response.isSuccessful = false;
    }
    else
    {
        response.errorLog = strdup("");  // Empty string indicates no error
        response.isSuccessful = true;
    }

    fclose(fp);
    curl_easy_cleanup(curl);

    return response;
}


    // Upload a file to Telegram Bot

}
