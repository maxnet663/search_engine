#include "include/converter_json.h"

#include <fstream> // ifs, ofs
#include <iostream> //cerr

#include "include/custom_functions.h"
#include "include/project_constants.h"

ConverterJSON::ConverterJSON(const std::string &jsons_dir) {
    if (!std::filesystem::is_directory(jsons_dir)) {
        throw std::filesystem::filesystem_error(
                "Wrong path"
                , jsons_dir
                , std::make_error_code(std::errc::no_such_file_or_directory));
    }

    // try to find paths to json files
    config_path = findFile(jsons_dir, CONFIG_FILE_NAME);
    if (config_path.empty()) {
        throw std::invalid_argument("Could not find " CONFIG_FILE_NAME);
    }

    requests_path = findFile(jsons_dir, REQUESTS_FILE_NAME);
    if (requests_path.empty()) {
        throw std::invalid_argument("Could not find " REQUESTS_FILE_NAME);
    }

    config = makeConfigJson(config_path);
    requests = makeRequestsJson(requests_path);
}

std::vector<std::string> ConverterJSON::getTextDocuments() const {
    return { config["files"].begin(), config["files"].end() };
}

inline int ConverterJSON::getResponsesLimit() const {
    return config["config"]["max_responses"];
}

std::vector<std::string> ConverterJSON::getRequests() const {

    std::vector<std::string> requests_list; // result data

    requests_list.reserve(requests["requests"].size());

    for (const auto &i : requests["requests"]) {
        std::string buf = to_string(i); // get request
        custom::formatString(buf); // format it
        requests_list.push_back(std::move(buf)); // write to requests_list
    }

    return requests_list;
}

void ConverterJSON::putAnswers(
        const std::vector<std::vector<RelativeIndex>> &answers) const {
    nlohmann::json json_file;

    // get round all the elements of the answers
    // and write them down in the final structure
    for (size_t i = 0; i < answers.size(); ++i) {
        std::string request = "request";

        // form number of the request
        std::string number(std::to_string(i + 1)); // 1 <= number <= 1000
        // fill spaces by zeroes
        number.insert(0, std::string(4 - number.length(), '0'));
        request.append(number); // get "request...."

        // if nothing is found for this request
        if (answers[i].empty()) {
            json_file["answers"][request]["result"] = !answers[i].empty();

        } else {

            //if only one answer is found
            if (answers[i].size() == 1) {
                json_file["answers"][request]["result"] = !answers.empty();
                json_file["answers"][request]["docid"] = answers[i][0].doc_id;
                json_file["answers"][request]["rank"] = answers[i][0].rank;

            } else {

                // if multiple results are found, add a field relevance
                json_file["answers"][request]["result"] = !answers[i].empty();

                auto limit = static_cast<size_t>(getResponsesLimit());
                for (size_t j = 0; j < limit && j < answers[i].size(); ++j) {
                    // element of relevance array
                    nlohmann::json field;
                    field = {
                            {"docid", answers[i][j].doc_id},
                            {"rank",  custom::round(answers[i][j].rank, 2)}
                    };
                    json_file["answers"][request]["relevance"].push_back(field);
                }
            }
        }
    }
    try {
        custom::writeJsonToFile(json_file, ANSWERS_FILE_NAME);
        custom::print_green("Result have written to " +
                            (std::filesystem::current_path()
                            / ANSWERS_FILE_NAME).string());
    }
    catch (std::filesystem::filesystem_error &ex) {
        custom::print_yellow(ex.what());
        std::string msg = "Could not write to the file \"answers.json\"\n"
                          "Result have written to \"answers_safe.json\"\n";
        custom::print_yellow(msg);
        custom::writeJsonToFile(json_file, EXCEPTION_ANSWERS_FILE_NAME);
        custom::print_green(
                (std::filesystem::current_path()
                / EXCEPTION_ANSWERS_FILE_NAME).string());
    }
}

 nlohmann::json ConverterJSON::openJson(const std::string &path) {
    if (!std::filesystem::exists(path) || !custom::isReadable(path)
        || std::filesystem::path(path).extension() != ".json") {
        return nullptr;
    }
    std::ifstream reader(path);
    if (!reader.is_open()) {
        throw std::filesystem::filesystem_error(
                "Can not read file " + path
                , path
                , custom::getFileName(path)
                , std::make_error_code(std::errc::bad_file_descriptor));
    }
    nlohmann::json result;
    reader >> result;
    reader.close();

    return result;
}

std::string ConverterJSON::findFile(const std::string &file_name) {
    auto dir_tree = std::filesystem::recursive_directory_iterator(
            std::filesystem::current_path());
    for (const auto &entry : dir_tree) {
        if (entry.path().filename() == file_name)
            return absolute(entry.path()).string();
    }
    return { };
}

std::string ConverterJSON::findFile(const std::string &dir
                                    , const std::string &file_name) {
    auto dir_tree = std::filesystem::recursive_directory_iterator(dir);
    for (const auto &entry : dir_tree) {
        if (entry.path().filename() == file_name)
            return absolute(entry.path()).string();
    }
    return { };
}

bool ConverterJSON::checkConfigFile(const std::filesystem::path &path) {
    return exists(path) && is_regular_file(path);
}

bool ConverterJSON::checkConfigProperties(const nlohmann::json &json_file) {

    // if file still do not open
    if (json_file == nullptr) {
        return false;
    }

    if (!json_file.contains("config") || json_file["config"].empty()) {
        throw std::invalid_argument("Config file is empty");
    }

    if (!json_file["config"].contains("max_responses")
        || json_file["config"]["max_responses"] <= 0) {
        throw std::invalid_argument("Wrong responses number");
    }

    if (!json_file.contains("files") || json_file["files"].empty()) {
        throw std::invalid_argument("No files to search");
    }

    // return true otherwise
    return true;
}

nlohmann::json ConverterJSON::makeConfigJson(const std::filesystem::path &path) {

    if (!checkConfigFile(path)) {
        throw std::filesystem::filesystem_error(
                "Config file is missing"
                , path
                , CONFIG_FILE_NAME
                , std::make_error_code(std::errc::no_such_file_or_directory)
                );
    }

    if (!custom::isReadable(path.string())) {
        throw std::filesystem::filesystem_error(
                CONFIG_FILE_NAME " permission denied"
                , path
                , CONFIG_FILE_NAME
                , std::make_error_code(std::errc::permission_denied)
        );
    }

    // make a json
    std::ifstream json_reader(path);
    nlohmann::json json_file;
    json_reader >> json_file;
    json_reader.close();

    // check if properties are valid
    checkConfigProperties(json_file); // throws if config has invalid properties

    return json_file;
}

bool ConverterJSON::checkRequestsFile(const std::filesystem::path &path) {
    return exists(path) && is_regular_file(path);
}

bool ConverterJSON::checkRequestsProperties(const nlohmann::json &json_file) {

    // throws if the contents of the files do not match the conditions

    if (!json_file.contains("requests") || json_file["requests"].empty()) {
        throw std::invalid_argument("Requests file is empty");
    }

    if (json_file["requests"].size() > MAX_REQUESTS_NUMBER) {
        throw std::invalid_argument("Requests number greater than "
                                    + std::to_string(MAX_REQUESTS_NUMBER));
    }

    for (const auto &i: json_file["requests"]) {
        if (custom::wordsCounter(to_string(i)) > MAX_REQUEST_WORDS_NUMBER)
            throw std::invalid_argument(
                    "One of Request's words number greater than "
                    + std::to_string(MAX_REQUEST_WORDS_NUMBER));
    }

    // return true otherwise
    return true;
}

nlohmann::json ConverterJSON::makeRequestsJson(const std::filesystem::path &path) {

    if (!custom::isReadable(path.string())) {
        throw std::filesystem::filesystem_error(
                REQUESTS_FILE_NAME " permission denied"
                , path
                , REQUESTS_FILE_NAME
                , std::make_error_code(std::errc::permission_denied));
    }

    if (!checkRequestsFile(path)) {
        throw std::filesystem::filesystem_error(
                "Requests file is missing"
                , path
                , REQUESTS_FILE_NAME
                , std::make_error_code(std::errc::no_such_file_or_directory));
    }

    // make a json
    std::ifstream json_reader(path);
    nlohmann::json json_file;
    json_reader >> json_file;
    json_reader.close();

    // check if properties are valid
    checkRequestsProperties(json_file); // throws if requests has invalid properties

    return json_file;
}




