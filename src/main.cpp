/* trat/main.cpp */
#include <iostream>
#include "../include/main.hpp"
#include "../include/bot.hpp"
#include "../include/parser.hpp"


int main() {
    const char* testCases[] = {
        "hello_world",  // Normal case
        "_world",       // Separator at the beginning
        "hello_",       // Separator at the end
        "_",            // Separator spans the whole string
        "hello world",  // No separator (should return NULL)
        ""              // Empty string (should return NULL)
    };
    const char* separator = "_";

    for (int i = 0; i < 6; ++i) {
        printf("Test case: \"%s\"\n", testCases[i]);
        trat::parser::PrefixSuffix* result = trat::parser::breakDownWord(testCases[i], separator);
        if (result) {
            printf("  Prefix: \"%s\"\n", result->prefix);
            printf("  Suffix: \"%s\"\n", result->suffix);
            trat::parser::PrefixSuffix_destroy(result);
        } else {
            printf("  The word cannot be broken down with the given separator.\n");
        }
    }

    return 0;
}
/*
int main() 
{
  const char* word = "HelloBalijbjhdfsjhfdsjhfsdhjhsdhsdhkjfhsjdhfjdsf_ojoifsjdioiohfuidshufdshusdhfuihsduifhuisdfhuisdfhWorld!";
  trat::SuffixPrefix *p_seprated_word = trat::breakDownWord(word, "_");
  std::cout << p_seprated_word -> suffix << "\n";
  std::cout << p_seprated_word -> prefix << "\n";
  trat::SuffixPrefix_destroy(p_seprated_word);
     
    int64_t chat_id = 6479143326;
    const char* token_buffer =  "7843656436:AAH6rROv5vGc2FiE16lGgD4-6A6OvbruW50";
    Bot bot = Bot(token_buffer, chat_id);
    bot.listen();

    return 0;
}
*/
