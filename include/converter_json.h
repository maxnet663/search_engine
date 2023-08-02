#ifndef CONVERTER_JSON_H
#define CONVERTER_JSON_H

#include "nlohmann/json.hpp" // json

#include <string> // string

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

};

#endif //CONVERTER_JSON_H
