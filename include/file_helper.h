#ifndef SEARCH_ENGINE
#define SEARCH_ENGINE

#include "nlohmann/json.hpp" // json

#include <string> // string


/*
 * Class with a functions for working with files
 * and formatting strings for later writing or
 * extracting from files
 */
class FileHelper {

public:

    /**
     * extract the content of the file
     * @param filename string containing path of file
     * @return string with text from the file
     */
    static std::string getFileText(const std::string &filename);

    /**
     * delete punctuation marks, extra spaces and lowercase all letters
     * @param s string to format
     * @return reference to format string
     */
    static void formatString(std::string &s);

    /**
     * counts number of words in string
     * @param s string
     * @return number of words in string
     */
    static int wordsCounter(const std::string &s);

    /**
     * lowercase all letters in s
     * @param s reference to string
     */
    static void toLowerCase(std::string &s);

    /**
     * delete all punctuation marks in s
     * @param s reference to string
     */
    static void deletePunctuationMarks(std::string &s);

    /**
     * delete all extra spaces
     * @param s reference to string
     */
    static void deleteExtraSpaces(std::string &s);

    /**
     * if file path exists overwriting it by file,
     * create new file in path otherwise
     * @param file file to write
     * @param path path to new file
     */
    static void writeJsonToFile(const nlohmann::json &jsonText, const std::string &path);

    /**
     * receives a path to a file or directory as input, returns its name
     * example:s ="/project/logs/config.txt", getFileName(s) returns config.txt
     * @param s path to file or directory
     * @return the name of directory or file
     */
    static std::string getFileName(std::string s);
};
#endif //SEARCH_ENGINE
