#include "converter_json.h"
#include "custom_limits.h"
#include "file_helper.h"

#include <fstream> // ifs, ofs
#include <iostream> // cerr
#include <regex> // regex, regex_match

//path to json files
static const std::filesystem::path jsonsDirectoryPath = FileHelper::findPath("JSONs");
static const std::string configFileName = "config.json";
static const std::string requestsFileName = "requests.json";
static const std::string resultsFileName = "results.json";

std::vector<std::string> ConverterJSON::GetTextDocuments() {

    std::vector<std::string> textDocuments; // result data

    //get list of files
    auto documentsList = getConfigJson()["files"];

    //reserve space
    textDocuments.reserve(documentsList.size());

    for (const auto &file : documentsList) {

        //get the path to the file
        auto pathToFile = FileHelper::findPath(file);

        if (std::filesystem::exists(pathToFile)) {
            try {
                textDocuments.push_back(FileHelper::getFileText(pathToFile));
            }
            catch (std::length_error &ex) { // catch the exception but don't break the execution
                std::cerr << ex.what() << std::endl; // warning
            }
        } else {
            std::cerr << pathToFile << "does not exist\n";
        }
    }

    return textDocuments;
}

int ConverterJSON::GetResponsesLimit() {
    return getConfigJson()["config"]["max_responses"];
}

std::vector<std::string> ConverterJSON::GetRequests() {

    std::vector<std::string> requests; // result data

    // get content of requests.json file
    auto requestsList = getRequestsJson();

    //reserve memory
    requests.reserve(requestsList["requests"].size());

    for (const auto &i : requestsList["requests"]) {
        std::string buf = to_string(i); // get request
        FileHelper::formatString(buf); // format it
        requests.push_back(std::move(buf)); // write to requests
    }

    return requests;
}

void ConverterJSON::putAnswers(
        std::vector<std::vector<std::pair<int, float>>> answers) {

    // result json to write in results.json
    nlohmann::json jsonFile;

    // get round all the elements of the answers
    // and write them down in the final structure
    for (size_t i = 0; i < answers.size(); ++i) {

        // field for requests
        std::string request = "request";

        // form number of the request
        std::string number(std::to_string(i + 1)); // 1 <= number <= 1000
        number.insert(0, std::string(4 - number.length(), '0')); // fill spaces by zeroes
        request.append(number); // get "request...."

        // if nothing is found for this request
        if (answers[i].empty()) {

            jsonFile["answers"][request]["result"] = !answers[i].empty();

        } else {

            //if only one answer is found
            if (answers[i].size() == 1) {
                jsonFile["answers"][request]["result"] = !answers.empty();
                jsonFile["answers"][request]["docid"] = answers[i].begin()->first;
                jsonFile["answers"][request]["rank"] = answers[i].begin()->second;

            } else {

                // if multiple results are found, add a field relevance
                jsonFile["answers"][request]["result"] = !answers[i].empty();

                // relevance field is represented as an array
                for (const auto &j: answers[i]) {

                    // element of relevance array
                    nlohmann::json newField;

                    newField = {
                            {"docid", j.first},
                            {"rank",  j.second}
                    };

                    // add at the end
                    jsonFile["answers"][request]["relevance"].push_back(newField);
                }
            }
        }
    }

    // write to the file
    FileHelper::writeJsonToFile(jsonFile
                                , jsonsDirectoryPath.string() + resultsFileName);
}

bool ConverterJSON::checkConfigFile() {

    // return true if file exist
    if (std::filesystem::exists(jsonsDirectoryPath.string() + configFileName)) {
        return true;
    }

    // throws otherwise
    throw std::filesystem::filesystem_error("Config file is missing"
                                      , jsonsDirectoryPath
                                      , configFileName
                                      , std::make_error_code(std::errc::no_such_file_or_directory));
}

bool ConverterJSON::checkConfigProperties(const nlohmann::json &jsonFile) {

    // throws if the contents of the files do not match the conditions

    if (!jsonFile.contains("config") || jsonFile["config"].empty()) {
        throw std::invalid_argument("Config file is empty");
    }

    if (!jsonFile["config"].contains("max_responses") || jsonFile["config"]["max_responses"] <= 0) {
        throw std::invalid_argument("Wrong responses number");
    }

    if (!jsonFile.contains("files") || jsonFile["files"].empty()) {
        throw std::invalid_argument("No files to search");
    }

    // return true otherwise
    return true;
}

nlohmann::json ConverterJSON::getConfigJson() {

    // throws if config does not exist
    checkConfigFile();

    // make a json
    std::ifstream jsonReader(jsonsDirectoryPath.string() + configFileName);
    nlohmann::json jsonFile;
    jsonReader >> jsonFile;
    jsonReader.close();

    // check if properties are valid
    checkConfigProperties(jsonFile); // throws if config has invalid properties

    return jsonFile;
}

bool ConverterJSON::checkRequestsFile() {

    // return true is file exists
    if (std::filesystem::exists(jsonsDirectoryPath.string() + requestsFileName)) {
        return true;
    }

    // throws otherwise
    throw std::filesystem::filesystem_error("Requests file is missing"
                    , jsonsDirectoryPath
                    , requestsFileName
                    , std::make_error_code(std::errc::no_such_file_or_directory));
}

bool ConverterJSON::checkRequestsProperties(const nlohmann::json &jsonFile) {

    // throws if the contents of the files do not match the conditions

    if (!jsonFile.contains("requests") || jsonFile["requests"].empty()) {
        throw std::invalid_argument("Requests file is empty");
    }

    if (jsonFile["requests"].size() > MAX_REQUESTS_NUMBER) {
        throw std::invalid_argument(std::string("Requests number greater than ")
                                    + std::to_string(MAX_REQUESTS_NUMBER));
    }

    for (const auto &i: jsonFile["requests"]) {
        if (FileHelper::wordsCounter(to_string(i)) > MAX_REQUEST_WORDS_NUMBER)
            throw std::invalid_argument(std::string("One of Request's words number greater than ")
                                        + std::to_string(MAX_REQUEST_WORDS_NUMBER));
    }

    // return true otherwise
    return true;
}

nlohmann::json ConverterJSON::getRequestsJson() {

    checkRequestsFile(); // throws if requests does not exist

    // make a json
    std::ifstream jsonReader(jsonsDirectoryPath.string() + requestsFileName);
    nlohmann::json jsonFile;
    jsonReader >> jsonFile;
    jsonReader.close();

    // check if properties are valid
    checkRequestsProperties(jsonFile); // throws if requests has invalid properties

    return jsonFile;
}




