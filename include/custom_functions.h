#ifndef SEARCH_ENGINE
#define SEARCH_ENGINE

#include "nlohmann/json.hpp" // json

#include <string> // string


/*
 * namespace with a functions for working with files
 * and formatting strings for later writing or
 * extracting from files
 */
namespace custom {

    /**
     * extract the content of the file
     * @param file_name string containing path of file
     * @return string with text from the file
     */
    std::string getFileText(const std::string &file_name);

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
    int wordsCounter(const std::string &s);

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
    std::vector<std::string> getUniqueWords(const std::string &text);

}
#endif //SEARCH_ENGINE
