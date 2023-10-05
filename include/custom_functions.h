#ifndef CUSTOM_FUNCTIONS
#define CUSTOM_FUNCTIONS

#include <string>

#include "nlohmann/json.hpp" // json

/**
 * The namespace defines helper functions for
 * for different tasks
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

    /**
     * Сolors printing to standard output in red
     * @param msg: message to printWithFormatting
     */
    void print_red(const std::string &msg, bool new_line = true);

    /**
     * Colors printing to standard output in green
     * @param msg: message to printWithFormatting
     */
    void print_green(const std::string &msg, bool new_line = true);

    /**
     * Colors printing to standard output in yellow
     * @param msg: message to printWithFormatting
     */
    void print_yellow(const std::string &msg, bool new_line = true);

    /**
     * Colors printing to standard output in blue
     * @param msg: message to printWithFormatting
     */
    void print_blue(const std::string &msg, bool new_line = true);

    /**
     * Colors printing to standard output in magenta
     * @param msg: message to printWithFormatting
     */
    void print_magenta(const std::string &msg, bool new_line = true);

    /**
     * Colors printing to standard output in cyan
     * @param msg: message to printWithFormatting
     */
     void print_cyan(const std::string &msg, bool new_line = true);
}
#endif //CUSTOM_FUNCTIONS
