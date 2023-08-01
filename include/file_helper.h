#ifndef SEARCH_ENGINE
#define SEARCH_ENGINE
#include <string>

/*
 * Class with a function for working with files
 * and formatting strings for later writing or
 * extracting from files
 */
class FileHelper {
public:
    /**
 * writes the contents of the file to ss
 * @param ss in: empty string stream. out:contents of the file
 * @param filename string containing path of file
 */
    static std::string getFileText(const std::string &filename);

    /**
     * check if file exists
     * @param path path to file
     * @return true if file with such name exists, false otherwise
     */
    static bool isFileExist(const std::string &path);

    /**
     * delete punctuation marks, extra spaces and lowercase all letters
     * @param s string to format
     * @return reference to formated string
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
};
#endif //SEARCH_ENGINE
