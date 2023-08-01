#ifndef CONVERTER_JSON_H
#define CONVERTER_JSON_H

#include "nlohmann/json.hpp"
#include <exception>
#include <string>
#include <utility>


// Custom exception class for throws in config checker
//class ConfigError : std::exception {
//public:
//    enum class ErrorCode {
//        CONFIG_ERROR = -10,
//        NAME_ERROR = -11,
//        VERSION_ERROR = -12,
//        MAX_RESPONSES_ERROR = -13
//    };
//    ConfigError(std::string msg, ErrorCode err) : message(std::move(msg)), errCode(err) {}
//    ConfigError(const ConfigError &other) : message(other.message), errCode(other.errCode) {}
//    ~ConfigError() override {}
//
//    const char* what() const noexcept override { return message.c_str(); }
//    const ErrorCode getErrCode() const noexcept { return  errCode; }
//private:
//    const std::string message;
//    const ErrorCode errCode;
//};

// Class for working with files
class ConverterJSON {
public:
    ConverterJSON() = default;

    /**
    * File content retrieval method
    * @return a list with the contents of the files listed
    * in config.json
    */
    static std::vector<std::string> GetTextDocuments();

    /**
    * The method reads the max_responses field to determine the limit
    * number of responses per request
    *@return max_responses
    */
    static int GetResponsesLimit();

    /**
    * Method for receiving requests from the requests.json file
    * @return a list of requests from the requests.json file
    */
    static std::vector<std::string> GetRequests();

    /**
    * Put search results in the answers.json file
    */
    static void putAnswers(std::vector<std::vector<std::pair<int, float>>> answers);

private:

    /**
     * check if config.json exists
     * @return true if config.json exists throws runtime_error otherwise
     */
    static bool checkConfigFile();

    /**
     * check if config properties is valid
     * @return true if contains the required values
     * throws invalid_argument otherwise
     */
     static bool checkConfigProperties(const nlohmann::json &jsonFile);

     /**
      * make a json from config.json
      * @return config json
      */
     static nlohmann::json getConfigJson();

    /**
     * check if requests properties is valid
     * @return true if contains the required values
     * throws invalid_argument otherwise
     */
    static bool checkRequestsProperties(const nlohmann::json &jsonFile);

    /**
     * check if requests.json exists
     * @return true if requests.json exists throws runtime_error otherwise
     */
    static bool checkRequestsFile();

     /**
      * make a json from requests.json
      * @return requests json
      */
     static nlohmann::json getRequestsJson();

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
};

#endif //CONVERTER_JSON_H
