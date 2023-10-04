#ifndef CUSTOM_FUNCTIONS
#define CUSTOM_FUNCTIONS

#include <string>

#include "nlohmann/json.hpp" // json

/**
 * The namespace defines helper functions for
 * string processing and text output
 */
namespace custom {

    /**
     * Counts number of words in string
     * @param s: string
     * @return number of words in string
     */
    size_t wordsCounter(const std::string &s);

    /**
     * The function generates a list of unique words from the text
     * @param text: string of one or more words
     * @return list of unique words
     */
    std::vector<std::string> getUniqueWords(const std::string &text);

    /**
     * Rounds the number up to n decimal place
     * @param num: number to round
     * @param precision: quantity with decimal place
     * @return rounded number
     */
    double round(double num, int precision);

}
#endif //CUSTOM_FUNCTIONS
