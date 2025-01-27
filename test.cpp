#include <iostream>
#include "include/parser.hpp"

/* All of getInvertString and getPrefixFromString got tested and are running without problem constructFilePath
 *
 *
 */
int main()
{
  
  using namespace trat; 
  char link[] = "https://www.google.com";
  char* prefix = parser::getPrefixFromString(link, "://" );
  if(!prefix)
  {
    printf("failure in prefixing\n");
  }
  std::cout << "what comes before the :// prefix: " <<  prefix << "\n";
  parser::cleanString(prefix);
  char* suffix = parser::getSuffixFromString(link, "." );
  std::cout << "what comes after the . suffix: " <<  suffix << "\n";
  parser::cleanString(suffix);
  return 0;
  
}
