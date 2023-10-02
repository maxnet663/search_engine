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
     * Receives a path to a file or directory as input, returns its name
     * example:s ="/project/logs/config.txt", getFileName(s) returns config.txt
     * @param s: path to file or directory
     * @return the name of directory or file
     */
    std::string getFileName(std::string s);

    /**
     * Count occurrences of the word in text
     * @param text: text
     * @param word: a word to count
     * @return number of occurrences word in text
     */
    size_t countOccurrences(std::string_view text, const std::string &word);

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
    void print_red(const std::string &msg);

    /**
     * Colors printing to standard output in green
     * @param msg: message to printWithFormatting
     */
    void print_green(const std::string &msg);

    /**
     * Colors printing to standard output in yellow
     * @param msg: message to printWithFormatting
     */
    void print_yellow(const std::string &msg);

    /**
     * Colors printing to standard output in blue
     * @param msg: message to printWithFormatting
     */
    void print_blue(const std::string &msg);

}
#endif //CUSTOM_FUNCTIONS
