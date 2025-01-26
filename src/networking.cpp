/* trat/src/networking.cpp */

#include "../include/networking.hpp"
#include <cstring>

#define NUMBER_OF_AVAILABLE_LINK_PREFIXES 6

namespace trat
{
  namespace networking
  {
   
    const char* link_prefixes[] = {
                                    "http",
                                    "https",
                                    "ftp",
                                    "fpts",
                                    "sftp",
                                    "file"
                                 };
    bool checkLinkValidity(const char* Link)
    {
      char* link_prefix = parser::getPrefixFromString(Link, ':');
      for (size_t i = 0; i < NUMBER_OF_AVAILABLE_LINK_PREFIXES; ++i)
      {
        if( strcmp(link_prefix, link_prefixes[i] ) == 0)
        {
          return true;
        }
      }
      return false;
    }//bool checkLinkValidity(const char* Link)
    
    void writeCallback(void *Data, size_t Size_Data_Element, size_t Number_Data_Elements, FILE *Output_Stream)
    {
      fwrite(Data, Size_Data_Element, Number_Data_Elements, Output_Stream);
    }

    NetworkingResponse *curlDownload(const char* Url, const char* Output_File_Path)
    {
      CURL* curl;
      CURLcode res;
      FILE* p_file = nullptr;
      NetworkingResponse *p_response = (NetworkingResponse*)malloc(sizeof(NetworkingResponse));

      clock_t start, end;

      curl = curl_easy_init();
      if (!curl)
      {
        p_response -> errorLog = parser::copyString("Failed to initialize curl");
        return p_response;
      }
      char* link = parser::copyString(Url);
      char* file_name = parser::getFileNameFromLink(link);
      
      parser::cleanString(link);

      if (!file_name)
      {
        p_response -> errorLog = parser::copyString("Failed to allocate memory for filename");
        return p_response;
      }
      
      char* full_file_name = parser::constructFilePath(Output_File_Path, file_name);
      parser::cleanString(file_name);

      p_file = fopen(full_file_name, "wb");
      if (!p_file)
      {
        curl_easy_cleanup(curl);
        p_response -> errorLog = parser::copyString("Failed to open file");
        parser::cleanString(full_file_name);
        return p_response;
      }

      curl_easy_setopt(curl, CURLOPT_URL, Url);
      curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
      curl_easy_setopt(curl, CURLOPT_WRITEDATA, p_file);

      start = clock();

      res = curl_easy_perform(curl);

      end = clock();

      p_response -> timeInSeconds = ((double)(end - start)) / CLOCKS_PER_SEC;

      if (res != CURLE_OK)
      {
        p_response -> errorLog = parser::copyString(curl_easy_strerror(res));
      }
      else
      {
        p_response -> errorLog = nullptr;
      }
      fclose(p_file);
      curl_easy_cleanup(curl);
      free(file_name);
      file_name = nullptr;

      return p_response;
    } //NetworkingResponse *curlDownload(const char* Url, const char* Output_File_Path)

    void NetworkingResponseDestroy(NetworkingResponse *P_Networking_Response)
    {
      if(!P_Networking_Response)  { return ;}
      if((P_Networking_Response -> errorLog) != nullptr) 
      { 
        parser::cleanString(P_Networking_Response -> errorLog);
      }
      free(P_Networking_Response);
      P_Networking_Response = nullptr;
    }
  
  } //namespace networking

} //namespace trat
