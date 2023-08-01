#include "converter_json.h"
#include <filesystem>
#include <system_error>
#include <fstream>
#include <sstream>
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

std::vector<std::string> ConverterJSON::GetTextDocuments() {
    auto filesList = getConfigJson()["files"];
    std::vector<std::string> textDocuments;
    textDocuments.reserve(filesList.size());

    for (const auto &file : filesList) {
        if (isFileExist(file)) {
            try {
                textDocuments.push_back(getFileText(file));
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
        formatString(buf);
        requests.push_back(std::move(buf));
    }
    return requests;
}

void ConverterJSON::putAnswers(
        std::vector<std::vector<std::pair<int, float>>> answers) {

}

bool ConverterJSON::checkConfigFile() {
    if (isFileExist(JsonsDirectoryPath.string() + configFileName)) {
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
    if (isFileExist(JsonsDirectoryPath.string() + requestsFileName)) {
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
        if (wordsCounter(to_string(i)) > MAX_REQUEST_WORDS_NUMBER)
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

std::string ConverterJSON::getFileText(const std::string &filename) {
    std::ifstream ifs(filename);

    //check words number in file
    if (std::distance(std::istream_iterator<std::string>(ifs)
            , std::istream_iterator<std::string>()) > MAX_WORDS_NUMBER) {
        throw std::length_error(std::string("number of words in file ")
                                + filename  + std::string(" greater than ")
                                + std::to_string(MAX_WORDS_NUMBER));
    }

    // back to the start of file
    ifs.clear();
    ifs.seekg(0);

    std::string buf; // buffer for word processing
    std::string text;//storage for text
    text.reserve(std::filesystem::file_size(filename));
    while (ifs) { // read one word at a time
        ifs >> buf;

        //check word's length
        if (buf.length() > MAX_WORD_LENGTH) {
            throw std::length_error(std::string("one of words from file:")
                                    + filename
                                    + std::string( " has length greater than ")
                                    + std::to_string(MAX_WORD_LENGTH));
        }

        //put the word to the storage
        text += buf + " ";
    }
    ifs.close();
    text.pop_back(); // delete last space
    formatString(text);
    return text; // returns formated string
}

bool ConverterJSON::isFileExist(const std::string &path) {
    return std::filesystem::exists(path);
}

void ConverterJSON::formatString(std::string &s)  {
    if (s.length() == 0) {
        return;
    }
    //delete punctuation marks
    s.erase(std::remove_if(s.begin()
                                , s.end()
                                , [](char &ch){
                                return std::ispunct(ch); })
            , s.end());

    //lowercase letters
    for (auto &ch : s) {
        if (isupper(ch)) {
            ch = tolower(ch);
        }
    }

    //delete extra spaces
    auto back = s.begin();
    auto front = s.begin();
    while (*front == ' ') {
        ++front;
    };
    while (*front) {
        if (*front != ' ' || *(front - 1) != ' ') {
            *back++ = *front;
        }
        front++;
    }
    *back = 0;
    s.resize(back - s.begin());
}

int ConverterJSON::wordsCounter(const std::string &s) {
    bool is_word = false;
    int words_counter = 0;
    for (const auto &ch: s) {
        if (!isalnum(ch) && is_word) {
            words_counter++;
            is_word = false;
            continue;
        }
        if (isalnum(ch)) {
            is_word = true;
            continue;
        }
    }
    return words_counter + is_word;
}



