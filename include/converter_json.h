#ifndef CONVERTER_JSON_H
#define CONVERTER_JSON_H

#include "nlohmann/json.hpp" // json

#include "include/search_server.h" // RelativeIndex
#include "include/observer.h"

#include <filesystem> //path
#include <string> // string

// Class for working with json files
class ConverterJSON : public IObserver{

    std::filesystem::path json_dir;
    nlohmann::json config;
    nlohmann::json requests;

public:

    ConverterJSON() : ConverterJSON(std::filesystem::current_path()) {}

    ConverterJSON(std::filesystem::path jsons_dir);

    inline std::filesystem::path getJsonDir() const { return json_dir; }

    inline void setJsonDir(std::filesystem::path new_path)
    { json_dir = std::move(new_path); }

    void update() override
    { config = getConfigJson(json_dir); requests = getRequestsJson(json_dir); }

    /**
    * @return a list with the paths to documents to search
    * in config.json
    */
    inline std::vector<std::string> getTextDocuments();

    /**
    * The method reads the max_responses field to determine the limit
    * number of responses per request
    *@return max_responses
    */
    inline int getResponsesLimit();

    /**
    * Method for receiving requests from the requests.json file
    * @return a list of requests from the requests.json file
    */
    std::vector<std::string> getRequests();

    /**
    * Put search results in the answers.json file
    */
     void putAnswers(std::vector<std::vector<RelativeIndex>> answers);

private:

    /**
     * check if config properties is valid
     * @return true if contains the required values
     * throws invalid_argument or filesystem_error otherwise
     */
    static bool checkConfigProperties(const nlohmann::json &json_file);

    /**
     * check if config.json exists
     * @param dir a directory inside which search file
     * @return true if config.json exists
     * false otherwise
     */
    static bool checkConfigFile(const std::filesystem::path &dir);

    /**
      * make a json object from config.json
      * @param dir a dir with config.json
      * @return json object
      */
    static nlohmann::json getConfigJson(const std::filesystem::path &dir);

    /**
     * check if requests properties is valid
     * @return true if contains the required values
     * throws invalid_argument otherwise
     */
    static bool checkRequestsProperties(const nlohmann::json &json_file);

    /**
     * check if requests.json exists
     * * @param dir a dir with requests.json
     * @return true if requests.json exists throws runtime_error otherwise
     */
    static bool checkRequestsFile(const std::filesystem::path &dir);

     /**
      * make a json from requests.json
      * @return requests json
      */
    static nlohmann::json getRequestsJson(const std::filesystem::path &dir);

};

#endif //CONVERTER_JSON_H
