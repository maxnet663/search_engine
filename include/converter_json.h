#ifndef CONVERTER_JSON_H
#define CONVERTER_JSON_H

#include <filesystem> //path
#include <string> // string

#include "nlohmann/json.hpp" // json

#include "include/search_server.h" // RelativeIndex

// Class for working with json files
class ConverterJSON {

    std::filesystem::path json_dir;
    nlohmann::json config;
    nlohmann::json requests;

public:

    ConverterJSON() : ConverterJSON(std::filesystem::current_path()) {}

    ConverterJSON(std::filesystem::path jsons_dir);

    inline std::filesystem::path getJsonDir() const { return json_dir; }

    inline void setJsonDir(std::filesystem::path new_path)
    { json_dir = std::move(new_path); }

    inline const nlohmann::json& getConfig() const { return config; }

    /**
    * @return a list with the paths to documents to search
    * in config.json
    */
    inline std::vector<std::string> getTextDocuments() const;

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
     void putAnswers(std::vector<std::vector<RelativeIndex>> answers) const;

     void updateConfig() { config = makeConfigJson(json_dir); }

     void updateRequests() {requests = makeRequestsJson(json_dir); }

     /**
      * creates json object from a file under path
      * !before creating check if file exists, perms to read
      * and file's extension(must be *.json)!
      * @param path path to the file
      * @return json object
      */
     static nlohmann::json openJson(const std::string &path);

private:

    /**
     * check if config properties is valid
     * @return true if contains the required values
     * throws invalid_argument otherwise
     */
    bool checkConfigProperties(const nlohmann::json &json_file);

    /**
     * check if config.json exists
     * @param dir a directory inside which search file
     * @return true if config.json exists
     * false otherwise
     */
    bool checkConfigFile(const std::filesystem::path &dir);

    /**
      * make a json object from config.json
      * @param dir a dir with config.json
      * @return json object made of config.json
      * throws filesystem_error or invalid_argument otherwise
      */
    nlohmann::json makeConfigJson(const std::filesystem::path &dir);

    /**
     * check if requests properties is valid
     * @return true if contains the required values
     * throws invalid_argument otherwise
     */
    bool checkRequestsProperties(const nlohmann::json &json_file);

    /**
     * check if requests.json exists
     * * @param dir a dir with requests.json
     * @return true if requests.json exists false otherwise
     */
    bool checkRequestsFile(const std::filesystem::path &dir);

    /**
     *
     * @param dir a dir with requests.json
     * @return json object made of request.json throws
     * filesystem_error or invalid_argument otherwise
     */
    nlohmann::json makeRequestsJson(const std::filesystem::path &dir);

};

#endif //CONVERTER_JSON_H
