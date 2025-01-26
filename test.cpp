#include <iostream>
#include "include/parser.hpp"

/* All of getInvertString and getPrefixFromString got tested and are running without problem constructFilePath
 *
 *
 */
int main()
{
  using namespace trat ;
  char* command = parser::copyString("/shell echo 'hello'");
  if(!command)
  {
    return 1;
  }
  char* link = parser::checkCommandAndExtractParemeter("/shell", command );
  if(!link)
  {
    return 1;
  }
  std::cout << link<< "\n";
  
  return 0;
}
