#ifndef CONVERTER_JSON_H
#define CONVERTER_JSON_H

#include "nlohmann/json.hpp" // json

#include "include/search_server.h" // RelativeIndex

#include <filesystem> //path
#include <string> // string

// Class for working with json files
class ConverterJSON {

    const std::filesystem::path json_dir;
    nlohmann::json config;
    nlohmann::json requests;

public:

    ConverterJSON() : ConverterJSON(std::filesystem::current_path()) {}

    ConverterJSON(std::filesystem::path jsons_dir);

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
     * check if config.json exists
     * @return true if config.json exists throws runtime_error otherwise
     */
    bool checkConfigFile();

    /**
     * check if config properties is valid
     * @return true if contains the required values
     * throws invalid_argument otherwise
     */
    bool checkConfigProperties(const nlohmann::json &json_file);

     /**
      * make a json from config.json
      * @return config json
      */
    nlohmann::json getConfigJson();

    /**
     * check if requests properties is valid
     * @return true if contains the required values
     * throws invalid_argument otherwise
     */
    bool checkRequestsProperties(const nlohmann::json &json_file);

    /**
     * check if requests.json exists
     * @return true if requests.json exists throws runtime_error otherwise
     */
    bool checkRequestsFile();

     /**
      * make a json from requests.json
      * @return requests json
      */
    nlohmann::json getRequestsJson();

};

#endif //CONVERTER_JSON_H
