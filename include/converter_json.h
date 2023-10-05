#ifndef CONVERTER_JSON_H
#define CONVERTER_JSON_H

#include <string> // string

#include "nlohmann/json.hpp" // json

#include "include/search_server.h" // RelativeIndex

/**
 * Represents json object
 */
typedef nlohmann::json json;

/**
 * Class for working with json files.
 * Works like RAII
 */
class ConverterJSON {

    json requests;
    json config;

public:

    /**
     * Constructs converter with searching configuration
     * files in jsons_dir
     * @param jsons_dir path to search in
     */
    explicit ConverterJSON(const std::string &jsons_dir);

    /**
     * Constructs converter from specified files,
     * file order doesn't matter
     * @param path_first path to config | requests
     * @param path_second path to config | requests
     */
    ConverterJSON(std::string path_first
                  , std::string path_second);

    /**
    * Method for receiving requests from the requests.json file
    * @return a list of requests from the requests.json file
    */
    std::vector<std::string> getRequests() const;

    /**
    * Method for receiving path to docs from the config.json file
    * @return a list with the paths to documents to search
    */
    std::vector<std::string> getDocumentsPaths() const;

    /**
    * The method reads the max_responses field to determine the limit
    * number of responses per request
    * @return max_responses
    */
    int getResponsesLimit() const;

    /**
     * Method writes answers to the file answers.json
     * @param answer: a data array containing answers to queries from
     * the database of indexed documents
     */
    void putAnswers(const std::vector<answer_t> &answers) const;

    /**
     * Creates json object from a file under path.
     * Before creating check if file exists, perms to read
     * and file's extension(must be *.json)
     * @param path: path to the file
     * @return json object if open is successfully,
     * empty json otherwise
     */
    static json openJson(const std::string &path);

    /**
     * If file path exists overwriting it by file,
     * create new file in path otherwise
     * @param file: json to write
     * @param path: path to new file
     */
    static int writeJsonToFile(json &json_obj, const std::string &path);

    /**
     * Searches for file_name in directory tree with root in dir
     * @param file_name: name of the file
     * @param dir: start dir, default: current dir
     * @return absolute path to a file or empty string
     */
    static std::string findFileRecursive(const std::string &file_name
                                         , const std::string &dir = ".");

    /**
     * Searches for file_name in current directory
     * and in directories such as json, jsons, JSON, JSONS
     * @param file_name name of the file to find
     * @return absolute path to a file or empty string
     */
    static std::string findFile(const std::string &file_name
                                , const std::string &dir = ".");

    /**
     * Print content of specified json file to standard output.
     * Structured fields prints in blue color, arrays in green,
     * elems of array in cyan, regular fields in magenta
     * @param source file to print out
     */
    static void printJson(const json &source);

private:

    /**
     * Check if config properties is valid
     * @return true if contains the required values
     * throws invalid_argument otherwise
     */
    bool checkConfigProperties(const json &json_file);

    /**
      * Make a json object from config.json
      * @param path: path to a config.json
      * @return json object made of config.json
      * throws filesystem_error or invalid_argument otherwise
      */
    json loadConfigJson(const std::string &path);

    /**
     * Check if requests properties is valid
     * @return true if contains the required values
     * throws invalid_argument otherwise
     */
    bool checkRequestsProperties(const json &json_file);

    /**
     * Make a json object from requests.json
     * @param path: to requests.json
     * @return json object made of request.json throws
     * filesystem_error or invalid_argument otherwise
     */
    json loadRequestsJson(const std::string &path);
};

#endif //CONVERTER_JSON_H
