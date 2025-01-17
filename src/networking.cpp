/* trat/src/networking.cpp */

#include "../include/networking.hpp"

namespace trat
{

  namespace networking
  {

    void setFileNameFromFullPath(const char *Full_Path, char **File_Name, char Separator)
    {
      const char *separatorPos = strrchr(Full_Path, Separator);

      if (separatorPos)
      {
        *File_Name = strdup(separatorPos + 1);
        if (*File_Name == nullptr)
        {
          *File_Name = nullptr;
        }
      }
      else
      {
        *File_Name = strdup(Full_Path);
        if (*File_Name == nullptr)
        {
          *File_Name = nullptr;
        }
      }
    }

    void writeCallback(void *Data, size_t Size_Data_Element, size_t Number_Data_Elements, FILE *Output_Stream)
    {
      fwrite(Data, Size_Data_Element, Number_Data_Elements, Output_Stream);
    }

    NetworkingResponse curlDownload(const char* Url, const char* Output_File_Path)
    {
      CURL* curl;
      CURLcode res;
      FILE* fp;
      NetworkingResponse response = {false, nullptr, 0.0};
      clock_t start, end;

      curl = curl_easy_init();
      if (!curl)
      {
        response.errorLog = strdup("Failed to initialize curl");
        return response;
      }

      char* filename;
      setFileNameFromFullPath(Output_File_Path, &filename, FILE_PATH_SEPARATOR);

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
        response.errorLog = strdup("");
        response.isSuccessful = true;
      }

      fclose(fp);
      curl_easy_cleanup(curl);
      free(filename);

      return response;
    }

  }

}
