#ifndef CUSTOM_FUNCTIONS
#define CUSTOM_FUNCTIONS

#include <cstdint> // uint8_t
#include <string>
#include <list>

#include "nlohmann/json.hpp" // json

/**
 * namespace with a functions for working with files
 * and formatting strings for later writing or
 * extracting from files
 */
namespace custom {

    /**
     * extract the content of the file
     * @param file_path path to the file
     * @return string with text from the file, throws
     */
    std::string getFileText(const std::string &file_path);

    /**
     * delete punctuation marks, extra spaces and lowercase all letters
     * @param s string to format
     * @return reference to format string
     */
    void formatString(std::string &s);

    /**
     * counts number of words in string
     * @param s string
     * @return number of words in string
     */
    size_t wordsCounter(const std::string &s);

    /**
     * lowercase all letters in s
     * @param s reference to string
     */
    void toLowerCase(std::string &s);

    /**
     * delete all punctuation marks in s
     * @param s reference to string
     */
    void deletePunctuationMarks(std::string &s);

    /**
     * delete all extra spaces
     * @param s reference to string
     */
    void deleteExtraSpaces(std::string &s);

    /**
     * if file path exists overwriting it by file,
     * create new file in path otherwise
     * @param file file to write
     * @param path path to new file
     */
    void writeJsonToFile(const nlohmann::json &json_text
                         , const std::string &path);

    /**
     * receives a path to a file or directory as input, returns its name
     * example:s ="/project/logs/config.txt", getFileName(s) returns config.txt
     * @param s path to file or directory
     * @return the name of directory or file
     */
    std::string getFileName(std::string s);

    /**
     * count occurrences of the word in text
     * @param text text
     * @param word a word to count
     * @return number of occurrences word in text
     */
    size_t countOccurrences(const std::string &text, const std::string &word);


    /**
     * the function generates a list of unique words from the text
     * @param text string of one or more words
     * @return list of unique words
     */
    std::list<std::string> getUniqueWords(const std::string &text);

    /**
     * rounds the number up to n decimal place
     * @param num number to round
     * @param precision
     * @return rounded number
     */
    double round(double num, int precision);

    /**
     * checks permission to read from a file
     * !only owner rights are checked!
     * @param file_path path to file
     * @return true if the file is readable by the owner
     * false otherwise
     */
    bool isReadable(const std::string &file_path);

    /**
     * checks permission to write to a file
     * !only owner rights are checked!
     * @param file_path path to file
     * @return true if the file is writeable by the owner
     * false otherwise
     */
    bool isWriteable(const std::string &file_path);

    /**
     * colors printing to standard output in red
     * @param msg message to print
     */
    void print_red(const std::string &msg);

    /**
     * colors printing to standard output in green
     * @param msg message to print
     */
    void print_green(const std::string &msg);

    /**
     * colors printing to standard output in yellow
     * @param msg message to print
     */
    void print_yellow(const std::string &msg);

    /**
     * colors printing to standard output in blue
     * @param msg message to print
     */
    void print_blue(const std::string &msg);

}
#endif //CUSTOM_FUNCTIONS
