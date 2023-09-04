#ifndef CONVERTER_JSON_H
#define CONVERTER_JSON_H

#include <filesystem> //path
#include <string> // string

#include "nlohmann/json.hpp" // json

#include "include/search_server.h" // RelativeIndex

// Class for working with json files
class ConverterJSON {

    std::string config_path;
    std::string requests_path;
    nlohmann::json config;
    nlohmann::json requests;

public:

    ConverterJSON() : ConverterJSON(
            std::filesystem::current_path().string()) {}

    ConverterJSON(const std::string &jsons_dir);

    /**
     * config getter
     * @return json object config.json
     */
    inline const nlohmann::json& getConfig() const { return config; }

    /**
     * config_path getter
     * @return path to current config.json
     */
    inline std::string getConfigPath() const { return config_path; }

    /**
     * requests_path getter
     * @return path to current requests.json
     */
    inline std::string getRequestsPath() const { return requests_path; }

    /**
    * @return a list with the paths to documents to search
    * in config.json
    */
    std::vector<std::string> getTextDocuments() const;

    /**
    * The method reads the max_responses field to determine the limit
    * number of responses per request
    *@return max_responses
    */
    inline int getResponsesLimit() const;

    /**
    * Method for receiving requests from the requests.json file
    * @return a list of requests from the requests.json file
    */
    std::vector<std::string> getRequests() const;

    /**
    * Put search results in the answers.json file
    */
    void putAnswers(
             const std::vector<std::vector<RelativeIndex>> &answers) const;

    void updateConfig()
    { config = makeConfigJson(config_path); }

    void updateRequests()
    { requests = makeRequestsJson(requests_path); }

    /**
     * creates json object from a file under path
     * !before creating check if file exists, perms to read
     * and file's extension(must be *.json)!
     * @param path path to the file
     * @return json object
     */
    static nlohmann::json openJson(const std::string &path);

    /**
     * searches for file_name
     * @param file_name
     * @return absolute path to file or empty string otherwise
     */
    static std::string findFile(const std::string &file_name);

    /**
     * searches for file_name in directory tree with root in dir
     * @param dir start dir
     * @param file_name name of the file
     * @return absolute path to a file or empty string
     */
    static std::string findFile(const std::string &dir
                                , const std::string &file_name);

private:

    /**
     * check if config properties is valid
     * @return true if contains the required values
     * throws invalid_argument otherwise
     */
    bool checkConfigProperties(const nlohmann::json &json_file);

    /**
     * check if config.json exists
     * @param path path to config.json
     * @return true if config.json exists
     * false otherwise
     */
    bool checkConfigFile(const std::filesystem::path &path);

    /**
      * make a json object from config.json
      * @param path path to a config.json
      * @return json object made of config.json
      * throws filesystem_error or invalid_argument otherwise
      */
    nlohmann::json makeConfigJson(const std::filesystem::path &path);

    /**
     * check if requests properties is valid
     * @return true if contains the required values
     * throws invalid_argument otherwise
     */
    bool checkRequestsProperties(const nlohmann::json &json_file);

    /**
     * check if requests.json exists
     * * @param path to requests.json
     * @return true if requests.json exists false otherwise
     */
    bool checkRequestsFile(const std::filesystem::path &path);

    /**
     * make a json object from requests.json
     * @param path to requests.json
     * @return json object made of request.json throws
     * filesystem_error or invalid_argument otherwise
     */
    nlohmann::json makeRequestsJson(const std::filesystem::path &path);

};

#endif //CONVERTER_JSON_H
