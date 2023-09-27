#include "include/converter_json.h"

#include <filesystem> //path
#include <fstream> // ifs, ofs
#include <iostream> // cerr

#include "include/custom_functions.h"
#include "include/file_reader.h"
#include "include/formatting.h"
#include "include/project_constants.h"

ConverterJSON::ConverterJSON(const PathType &jsons_dir) {
    if (!std::filesystem::is_directory(jsons_dir)) {
        throw std::filesystem::filesystem_error(
                "Wrong path"
                , jsons_dir
                , std::make_error_code(std::errc::not_a_directory));
    }

    // try to find paths to json files
    config_path = findFile(CONFIG_FILE_NAME, jsons_dir );
    if (config_path.empty()) {
        throw std::invalid_argument("Could not find " CONFIG_FILE_NAME);
    }
#ifndef TEST
    custom::print_green("config.json found successfully");
#endif

    requests_path = findFile(REQUESTS_FILE_NAME, jsons_dir);
    if (requests_path.empty()) {
        throw std::invalid_argument("Could not find " REQUESTS_FILE_NAME);
    }
#ifndef TEST
    custom::print_green("requests.json found successfully");
#endif

    config = loadConfigJson(config_path);
    requests = loadRequestsJson(requests_path);
}

ConverterJSON::ConverterJSON(PathType conf_p, PathType req_p)
: config_path(std::move(conf_p)), requests_path(std::move(req_p)) {
    config = loadConfigJson(config_path);
#ifndef TEST
    custom::print_green("config.json found successfully");
#endif
    requests = loadRequestsJson(requests_path);
#ifndef TEST
    custom::print_green("requests.json found successfully");
#endif
}

PathsList ConverterJSON::getTextDocuments() const {
    if (config.is_null())
        return { };
    else
        return { config["files"].begin(), config["files"].end() };
}

RequestsList ConverterJSON::getRequests() const {
    if (requests.is_null())
        return { };

    RequestsList requests_list; // result data
    requests_list.reserve(requests["requests"].size());

    for (const auto &req : requests["requests"]) {
        auto utf_text = to_string(req); // get request
        auto uc_text = format::unicode::makeUnicodeString(utf_text);
        format::unicode::convertToPlainText(uc_text);
        auto formatted_text = format::unicode::makeUtfString(uc_text);
        requests_list.push_back(std::move(formatted_text)); // write to requests_list
    }

    return requests_list;
}

int ConverterJSON::getResponsesLimit() const {
    if (config.is_null())
        return -1;
    else
        return config["config"]["max_responses"];
}

void ConverterJSON::putAnswers(const AnswersLists &answers) const {
    json ans_json;

    // get round all the elements of the answers
    // and write them down in the final structure
    for (size_t i = 0; i < answers.size(); ++i) {
        std::string request = "request";

        // form number of the request
        std::string number(std::to_string(i + 1)); // 1 <= number <= 1000
        // fill spaces by zeroes
        number.insert(0, std::string(4 - number.length(), '0'));
        request.append(number); // get "requestNNNN"

        bool search_result = !answers[i].empty();

        if (!search_result) { // if nothing is found for this request

            ans_json["answers"][request]["result"] = search_result;

        } else {

            if (answers[i].size() == 1) { // if only one answer is found
                ans_json["answers"][request]["result"] = search_result;
                ans_json["answers"][request]["docid"] = answers[i][0].doc_id;
                ans_json["answers"][request]["rank"] = answers[i][0].rank;

            } else { // if multiple results are found, add a field relevance

                ans_json["answers"][request]["result"] = search_result;
                auto limit = static_cast<size_t>(getResponsesLimit());
                for (size_t j = 0; j < limit && j < answers[i].size(); ++j) {
                    json field = {
                            {"docid", answers[i][j].doc_id},
                            {"rank",  custom::round(answers[i][j].rank, 2)}
                    };
                    ans_json["answers"][request]["relevance"].push_back(field);
                }
            }
        }
    }
#ifdef TEST
    writeJsonToFile(ans_json, ANSWERS_FILE_NAME);
#else
    auto written = writeJsonToFile(ans_json, ANSWERS_FILE_NAME);
    if (written == 1)
        std::cout << "Search results have written to " << ANSWERS_FILE_NAME
                  << std::endl;
    if (written == 0)
        custom::print_yellow("Could not write search results to "
                             ANSWERS_FILE_NAME);
#endif
}

void ConverterJSON::updateConfig(const PathType &path) {
    json new_config;
    try {
        if (path.empty()) {
            new_config = loadConfigJson(config_path);
        } else {
            new_config = loadConfigJson(path);
            config_path = path;
        }
        config = std::move(new_config);
   }
    catch (std::exception &ex) {
        custom::print_yellow(ex.what());
    }
}

void ConverterJSON::updateRequests(const PathType &path) {
    json new_requests;
    try {
        if (path.empty()) {
            new_requests = loadRequestsJson(requests_path);
        } else {
            new_requests = loadRequestsJson(path);
            requests_path = path;
        }
        requests = std::move(new_requests);
    }
    catch (std::exception &ex) {
        custom::print_yellow(ex.what());
    }
}

json ConverterJSON::openJson(const PathType &path) {
    if (!std::filesystem::exists(path) || !FileReader::isReadable(path)) {
        custom::print_yellow(
                "Could not open file " + path + ". Check the file");
        return nullptr;
    }
    if (std::filesystem::path(path).extension() != ".json") {
        custom::print_yellow("The file " + path + " must be in json format");
        return nullptr;
    }
    std::ifstream reader(path);
    if (!reader.is_open()) {
        custom::print_yellow("Attempt to open the file " + path + " failed");
        return nullptr;
    }
    json result;
    reader >> result;
    reader.close();

    return result;
}

int ConverterJSON::writeJsonToFile(json &json_obj, const std::string &path) {
#ifndef TEST
    if (std::filesystem::exists(path)) {
        if (std::filesystem::is_directory(path)) {
            custom::print_yellow(path + " is a directory");
            return 0;
        }
        if (!FileReader::isWriteable(path)) {
            custom::print_yellow("Can not write to the file "
                                + path
                                + " permission denied");
            return 0;
        } else {
            std::string input;
            std::cout << "File " + path + " already exists" << std::endl
                      << "Do you want to overwrite it?[y/n]: ";
            std::getline(std::cin, input);
            format::utf::deleteExtraSpaces(input);
            format::utf::toLowerCase(input);
            if (input == "n" || input == "no")
                return -1;
        }
    }
#endif
    std::ofstream dest(path, std::ios::out | std::ios::trunc);
    dest << std::setw(2) << json_obj;
    dest.close();
    return 1;
}

PathType ConverterJSON::findFile(const std::string &file_name
                                    , const PathType &dir) {
    auto dir_tree = std::filesystem::recursive_directory_iterator(dir);
    for (const auto &entry : dir_tree) {
        if (entry.path().filename() == file_name)
            return absolute(entry.path()).string();
    }
    return { };
}

bool ConverterJSON::checkConfigProperties(const json &json_file) {

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

json ConverterJSON::loadConfigJson(const PathType &path) {
    auto json_file = openJson(path);
    if (json_file.is_null()) {
        throw std::filesystem::filesystem_error(
                "Config file is missing"
                , path
                , CONFIG_FILE_NAME
                , std::make_error_code(std::errc::no_such_file_or_directory));
    }
    checkConfigProperties(json_file); // throws if config has invalid properties

    return json_file;
}

bool ConverterJSON::checkRequestsProperties(const json &json_file) {

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

json ConverterJSON::loadRequestsJson(const PathType &path) {
    auto json_file = openJson(path);
    if (json_file.is_null()) {
        throw std::filesystem::filesystem_error(
                "Requests file is missing"
                , path
                , REQUESTS_FILE_NAME
                , std::make_error_code(std::errc::no_such_file_or_directory));
    }
    // throws if requests has invalid properties
    checkRequestsProperties(json_file);

    return json_file;
}
