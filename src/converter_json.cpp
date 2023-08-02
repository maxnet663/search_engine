#include "converter_json.h"
#include "file_helper.h"

#include <filesystem> //path
#include <fstream> //ifs, ofs
#include <iostream>

//Constants
#define MAX_WORDS_NUMBER 1000
#define MAX_WORD_LENGTH 100
#define MAX_REQUESTS_NUMBER 1000
#define MAX_REQUEST_WORDS_NUMBER 10

//path with json files
static const std::filesystem::path JsonsDirectoryPath("../JSONs/");
static const std::string configFileName = "config.json";
static const std::string requestsFileName = "requests.json";
static const std::string resultsFileName = "results.json";

std::vector<std::string> ConverterJSON::GetTextDocuments() {
    auto filesList = getConfigJson()["files"];
    std::vector<std::string> textDocuments;
    textDocuments.reserve(filesList.size());

    for (const auto &file : filesList) {
        if (FileHelper::isFileExist(file)) {
            try {
                textDocuments.push_back(FileHelper::getFileText(file));
            }
            catch (std::length_error &ex) {
                std::cerr << ex.what() << std::endl;
            }
        } else {
            std::cerr << file << "does not exist\n";
        }
    }
    return textDocuments;
}

int ConverterJSON::GetResponsesLimit() {
    return getConfigJson()["config"]["max_responses"];
}

std::vector<std::string> ConverterJSON::GetRequests()  {
    auto jsonFile = getRequestsJson();
    std::vector<std::string> requests;
    requests.reserve(jsonFile["requests"].size());
    for (const auto &i : jsonFile["requests"]) {
        std::string buf = to_string(i);
        FileHelper::formatString(buf);
        requests.push_back(std::move(buf));
    }
    return requests;
}

void ConverterJSON::putAnswers(
        std::vector<std::vector<std::pair<int, float>>> answers) {
    nlohmann::json jsonFile;
    for (size_t i = 0; i < answers.size(); ++i) {
        std::string request = "request";
        std::string number(std::to_string(i + 1));
        number.insert(0, std::string(4 - number.length(), '0'));
        request.append(number);
        if (answers[i].empty()) {
            jsonFile["answers"][request]["result"] = !answers[i].empty();
        } else {
            if (answers[i].size() == 1) {
                jsonFile["answers"][request]["result"] = !answers.empty();
                jsonFile["answers"][request]["docid"] = answers[i].begin()->first;
                jsonFile["answers"][request]["rank"] = answers[i].begin()->second;
            } else {
                jsonFile["answers"][request]["result"] = !answers[i].empty();
                for (const auto &j: answers[i]) {
                    nlohmann::json newField;
                    newField = {
                            {"docid", j.first},
                            {"rank",  j.second}
                    };
                    jsonFile["answers"][request]["relevance"].push_back(newField);
                }
            }
        }
    }
    FileHelper::writeJsonToFile(jsonFile
                                , JsonsDirectoryPath.string() + resultsFileName);
}

bool ConverterJSON::checkConfigFile() {
    if (FileHelper::isFileExist(JsonsDirectoryPath.string() + configFileName)) {
        return true;
    }
    throw std::filesystem::filesystem_error("Config file is missing"
                                      , JsonsDirectoryPath
                                      , configFileName
                                      , std::make_error_code(std::errc::no_such_file_or_directory));
}

bool ConverterJSON::checkConfigProperties(const nlohmann::json &jsonFile) {
    if (!jsonFile.contains("config") || jsonFile["config"].empty()) {
        throw std::invalid_argument("Config file is empty");
    }
    if (!jsonFile["config"].contains("max_responses") || jsonFile["config"]["max_responses"] <= 0) {
        throw std::invalid_argument("Wrong responses number");
    }
    if (!jsonFile.contains("files") || jsonFile["files"].empty()) {
        throw std::invalid_argument("No files to search");
    }
    return true;
}

nlohmann::json ConverterJSON::getConfigJson() {
    checkConfigFile(); // throws if config does not exist

    // make a json
    std::ifstream jsonReader(JsonsDirectoryPath.string() + configFileName);
    nlohmann::json jsonFile;
    jsonReader >> jsonFile;
    jsonReader.close();

    // check if properties are valid
    checkConfigProperties(jsonFile); // throws if config has invalid properties

    return jsonFile;
}

bool ConverterJSON::checkRequestsFile() {
    if (FileHelper::isFileExist(JsonsDirectoryPath.string() + requestsFileName)) {
        return true;
    }
    throw std::filesystem::filesystem_error("Requests file is missing"
                    , JsonsDirectoryPath
                    , requestsFileName
                    , std::make_error_code(std::errc::no_such_file_or_directory));
}

bool ConverterJSON::checkRequestsProperties(const nlohmann::json &jsonFile) {
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
    return true;
}

nlohmann::json ConverterJSON::getRequestsJson() {
    checkRequestsFile();

    std::ifstream jsonReader(JsonsDirectoryPath.string() + requestsFileName);
    nlohmann::json jsonFile;
    jsonReader >> jsonFile;
    jsonReader.close();
    checkRequestsProperties(jsonFile);
    return jsonFile;
}




