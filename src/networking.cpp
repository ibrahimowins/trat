/* trat/src/networking.cpp */

#include "../include/networking.hpp"

namespace trat
{
  namespace networking
  {
    void setFileNameFromFullPath(const char *Full_Path, char **File_Name, char Separator)
    {
      const char *separator_pos = strrchr(Full_Path, Separator);

      if (separator_pos)
      {
        *File_Name = strdup(separator_pos + 1);
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
      FILE* p_file = nullptr;
      NetworkingResponse response = {false, nullptr, 0.0};
      clock_t start, end;

      curl = curl_easy_init();
      if (!curl)
      {
        response.errorLog = strdup("Failed to initialize curl");
        return response;
      }
      char* file_name;
      setFileNameFromFullPath(Output_File_Path, &file_name, FILE_PATH_SEPARATOR);

      if (!file_name)
      {
        response.errorLog = strdup("Failed to allocate memory for filename");
        return response;
      }

      p_file = fopen(file_name, "wb");
      if (!p_file)
      {
        curl_easy_cleanup(curl);
        response.errorLog = strdup("Failed to open file");
        free(file_name);
        return response;
      }

      curl_easy_setopt(curl, CURLOPT_URL, Url);
      curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
      curl_easy_setopt(curl, CURLOPT_WRITEDATA, p_file);

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
      fclose(p_file);
      curl_easy_cleanup(curl);
      free(file_name);
      file_name = nullptr;

      return response;
    }
  }
}
