#ifndef CONVERTER_JSON_H
#define CONVERTER_JSON_H

#include <filesystem> //path
#include <string> // string

#include "nlohmann/json.hpp" // json

#include "include/search_server.h" // RelativeIndex

typedef std::string PathType;
typedef std::vector<std::string> RequestsList;
typedef std::vector<std::string> PathsList;
typedef nlohmann::json json;
typedef std::vector<std::vector<RelativeIndex>> AnswersLists;

// Class for working with json files
class ConverterJSON {
    PathType config_path;
    PathType requests_path;
    json config;
    json requests;

public:

    ConverterJSON() : config(nullptr), requests(nullptr) {};

    ConverterJSON(const PathType &jsons_dir);

    ConverterJSON(PathType conf_p, PathType req_p);

    /**
     * config getter
     * @return json object config.json
     */
    const json& getConfig() const { return config; }

    /**
    * Method for receiving requests from the requests.json file
    * @return a list of requests from the requests.json file
    */
    RequestsList getRequests() const;

    /**
    * @return a list with the paths to documents to search
    * in config.json
    */
    PathsList getTextDocuments() const;

    /**
    * The method reads the max_responses field to determine the limit
    * number of responses per request
    *@return max_responses
    */
    int getResponsesLimit() const;

    /**
     * Method writes answers to the file answers.json in json format
     * @param answer a data array containing answers to queries to
     * the database of indexed documents
     */
    void putAnswers(const AnswersLists &answers) const;

   /**
    * overwrites the current config file according to the path
    * @param path path to json file
    */
    void updateConfig(const PathType &path = "");

    /**
     * overwrites the current requests file according to the path
     * @param path path to json file
     */
    void updateRequests(const PathType &path = "");

    /**
     * config_path getter
     * @return config_path member
     */
    PathType getConfigPath() const { return config_path; }

    /**
     * requests_path getter
     * @return requests_path member
     */
    PathType getRequestsPath() const {return requests_path; }

    /**
     * creates json object from a file under path
     * !before creating check if file exists, perms to read
     * and file's extension(must be *.json)!
     * @param path path to the file
     * @return json object
     */
    static json openJson(const PathType &path);

    /**
     * searches for file_name in directory tree with root in dir
     * @param file_name name of the file
     * @param dir start dir
     * @return absolute path to a file or empty string
     */
    static PathType findFile(const std::string &file_name
                                , const PathType &dir = ".");

private:

    /**
     * check if config properties is valid
     * @return true if contains the required values
     * throws invalid_argument otherwise
     */
    bool checkConfigProperties(const json &json_file);

    /**
      * make a json object from config.json
      * @param path path to a config.json
      * @return json object made of config.json
      * throws filesystem_error or invalid_argument otherwise
      */
    json makeConfigJson(const PathType &path);

    /**
     * check if requests properties is valid
     * @return true if contains the required values
     * throws invalid_argument otherwise
     */
    bool checkRequestsProperties(const json &json_file);

    /**
     * make a json object from requests.json
     * @param path to requests.json
     * @return json object made of request.json throws
     * filesystem_error or invalid_argument otherwise
     */
    json makeRequestsJson(const PathType &path);
};

#endif //CONVERTER_JSON_H
