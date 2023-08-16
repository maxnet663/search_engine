#include "converter_json.h"

#include "custom_functions.h"
#include "project_constants.h"

#include <fstream> // ifs, ofs
#include <iostream> // cerr

std::vector<std::string> ConverterJSON::GetTextDocuments() {

    std::vector<std::string> documents_text; // result data

    //get list of files
    auto documents_list = getConfigJson()["files"];

    //reserve space
    documents_text.reserve(documents_list.size());

    for (const auto &file : documents_list) {

        auto file_path =
                std::string(RESOURCES_DIR) + custom::getFileName(file);

        if (std::filesystem::exists(file_path)) {
            try {
                documents_text.push_back(custom::getFileText(file_path));
            }
            catch (std::length_error &ex) { // catch the exception but don't break the execution
                std::cerr << ex.what() << std::endl; // warning
            }
        } else {
            std::cerr << file_path << "does not exist\n";
        }
    }

    return documents_text;
}

inline int ConverterJSON::GetResponsesLimit() {
    return getConfigJson()["config"]["max_responses"];
}

std::vector<std::string> ConverterJSON::GetRequests() {

    std::vector<std::string> requests; // result data

    // get content of requests.json file
    auto requests_list = getRequestsJson();

    //reserve memory
    requests.reserve(requests_list["requests"].size());

    for (const auto &i : requests_list["requests"]) {
        std::string buf = to_string(i); // get request
        custom::formatString(buf); // format it
        requests.push_back(std::move(buf)); // write to requests
    }

    return requests;
}

void ConverterJSON::putAnswers(
        std::vector<std::vector<RelativeIndex>> answers) {

    // result json to write in results.json
    nlohmann::json json_file;

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

            json_file["answers"][request]["result"] = !answers[i].empty();

        } else {

            //if only one answer is found
            if (answers[i].size() == 1) {
                json_file["answers"][request]["result"] = !answers.empty();
                json_file["answers"][request]["docid"] = answers[i].begin()->doc_id;
                json_file["answers"][request]["rank"] = answers[i].begin()->rank;

            } else {

                // if multiple results are found, add a field relevance
                json_file["answers"][request]["result"] = !answers[i].empty();

                // relevance field is represented as an array
                if (answers.size() > GetResponsesLimit()) {
                    answers.resize(GetResponsesLimit());
                }
                for (const auto &record: answers[i]) {

                    // element of relevance array
                    nlohmann::json new_field;
                    new_field = {
                            {"docid", record.doc_id},
                            {"rank",  record.rank}
                    };

                    json_file["answers"][request]["relevance"].push_back(new_field);
                }
            }
        }
    }

    // write to the file
    custom::writeJsonToFile(json_file, JSONS_DIR RESULTS_FILE_NAME);
}

bool ConverterJSON::checkConfigFile() {

    // return true if file exist
    if (std::filesystem::exists(JSONS_DIR CONFIG_FILE_NAME)) {
        return true;
    }

    // throws otherwise
    throw std::filesystem::filesystem_error("Config file is missing"
                                      , JSONS_DIR
                                      , CONFIG_FILE_NAME
                                      , std::make_error_code(std::errc::no_such_file_or_directory));
}

bool ConverterJSON::checkConfigProperties(const nlohmann::json &json_file) {

    // throws if the contents of the files do not match the conditions

    if (!json_file.contains("config") || json_file["config"].empty()) {
        throw std::invalid_argument("Config file is empty");
    }

    if (!json_file["config"].contains("max_responses") || json_file["config"]["max_responses"] <= 0) {
        throw std::invalid_argument("Wrong responses number");
    }

    if (!json_file.contains("files") || json_file["files"].empty()) {
        throw std::invalid_argument("No files to search");
    }

    // return true otherwise
    return true;
}

nlohmann::json ConverterJSON::getConfigJson() {

    // throws if config does not exist
    checkConfigFile();

    // make a json
    std::ifstream json_reader(JSONS_DIR CONFIG_FILE_NAME);
    nlohmann::json json_file;
    json_reader >> json_file;
    json_reader.close();

    // check if properties are valid
    checkConfigProperties(json_file); // throws if config has invalid properties

    return json_file;
}

bool ConverterJSON::checkRequestsFile() {

    // return true is file exists
    if (std::filesystem::exists(JSONS_DIR REQUESTS_FILE_NAME)) {
        return true;
    }

    // throws otherwise
    throw std::filesystem::filesystem_error("Requests file is missing"
                    , JSONS_DIR
                    , REQUESTS_FILE_NAME
                    , std::make_error_code(std::errc::no_such_file_or_directory));
}

bool ConverterJSON::checkRequestsProperties(const nlohmann::json &json_file) {

    // throws if the contents of the files do not match the conditions

    if (!json_file.contains("requests") || json_file["requests"].empty()) {
        throw std::invalid_argument("Requests file is empty");
    }

    if (json_file["requests"].size() > MAX_REQUESTS_NUMBER) {
        throw std::invalid_argument(std::string("Requests number greater than ")
                                    + std::to_string(MAX_REQUESTS_NUMBER));
    }

    for (const auto &i: json_file["requests"]) {
        if (custom::wordsCounter(to_string(i)) > MAX_REQUEST_WORDS_NUMBER)
            throw std::invalid_argument(std::string("One of Request's words number greater than ")
                                        + std::to_string(MAX_REQUEST_WORDS_NUMBER));
    }

    // return true otherwise
    return true;
}

nlohmann::json ConverterJSON::getRequestsJson() {

    checkRequestsFile(); // throws if requests does not exist

    // make a json
    std::ifstream json_reader(JSONS_DIR REQUESTS_FILE_NAME);
    nlohmann::json json_file;
    json_reader >> json_file;
    json_reader.close();

    // check if properties are valid
    checkRequestsProperties(json_file); // throws if requests has invalid properties

    return json_file;
}




