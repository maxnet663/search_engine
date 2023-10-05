#include "include/converter_json.h"

#include <filesystem> //path
#include <fstream> // ifs, ofs
#include <iostream> // cout
#include <regex> // reg_match, regex

#include "include/custom_functions.h"
#include "include/file_reader.h"
#include "include/formatting.h"
#include "include/project_constants.h"

namespace fs = std::filesystem;

ConverterJSON::ConverterJSON(const std::string &jsons_dir) {
    if (!fs::is_directory(jsons_dir)) {
        throw fs::filesystem_error(
                "Wrong path"
                , jsons_dir
                , std::make_error_code(std::errc::not_a_directory));
    }

    // try to find paths to json files
    auto config_path = findFile(CONFIG_FILE_NAME, jsons_dir);
    if (config_path.empty()) {
        throw std::invalid_argument("Could not find " CONFIG_FILE_NAME);
    }
#ifndef TEST
    custom::print_green("config.json found successfully");
#endif

    auto requests_path = findFile(REQUESTS_FILE_NAME, jsons_dir);
    if (requests_path.empty()) {
        throw std::invalid_argument("Could not find " REQUESTS_FILE_NAME);
    }
#ifndef TEST
    custom::print_green("requests.json found successfully");
#endif

    config = loadConfigJson(config_path);
    requests = loadRequestsJson(requests_path);
}

ConverterJSON::ConverterJSON(std::string path_first
                             , std::string path_second) {
    std::regex config_pattern("config\\.json$", std::regex::icase);
    std::regex requests_pattern("requests\\.json$", std::regex::icase);

    // if none of the paths match pattern for config.json
    if (!std::regex_search(path_first, config_pattern)
    && !std::regex_search(path_second, config_pattern)) {
        throw std::invalid_argument("None of the transferred files "
                                    "look like config.json: "
                                    + path_first + " "
                                    + path_second);
    }

    // if none of the paths match pattern for requests.json
    if (!std::regex_search(path_first, requests_pattern)
    && !std::regex_search(path_second, requests_pattern)) {
        throw std::invalid_argument("None of the transferred files "
                                    "look like requests.json: "
                                    + path_first + " "
                                    + path_second);
    }

    std::string config_path;
    std::string requests_path;

    if (std::regex_search(path_first, config_pattern)) {
        std::swap(config_path, path_first);
        std::swap(requests_path, path_second);
    } else {
        std::swap(config_path, path_second);
        std::swap(requests_path, path_first);
    }

    config = loadConfigJson(config_path);
#ifndef TEST
    custom::print_green("config.json found successfully");
#endif
    requests = loadRequestsJson(requests_path);
#ifndef TEST
    custom::print_green("requests.json found successfully");
#endif
}

std::vector<std::string> ConverterJSON::getDocumentsPaths() const {
    if (config.is_null())
        return { };
    else
        return { config["files"].begin(), config["files"].end() };
}

std::vector<std::string> ConverterJSON::getRequests() const {
    if (requests.is_null())
        return { };

    std::vector<std::string> requests_list;
    requests_list.reserve(requests["requests"].size());

    for (const auto &req : requests["requests"]) {
        auto utf_text = to_string(req);
        auto uc_text = format::unicode::makeUnicodeString(utf_text);
        format::unicode::convertToPlainText(uc_text);
        requests_list.push_back(format::unicode::makeUtfString(uc_text));
    }
    return requests_list;
}

int ConverterJSON::getResponsesLimit() const {
    if (config.is_null())
        return -1;
    else
        return config["config"]["max_responses"];
}

void ConverterJSON::putAnswers(const std::vector<answer_t> &answers) const {
    json ans_json;
    for (size_t i = 0; i < answers.size(); ++i) {
        std::string request = "request";

        // form number of the request
        std::string number(std::to_string(i + 1)); // 1 <= number <= 1000
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
    writeJsonToFile(ans_json, ANSWERS_FILE_NAME);
    custom::print_green("Search results have written to " ANSWERS_FILE_NAME);
}

json ConverterJSON::openJson(const std::string &path) {
    if (!fs::exists(path) || !FileReader::isReadable(path)) {
        custom::print_yellow("Could not open file "
                            + path
                            + ". Check the file");
        return nullptr;
    }
    if (fs::path(path).extension() != ".json") {
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
    if (fs::exists(path)) {
        if (fs::is_directory(path)) {
            custom::print_yellow(path + " is a directory");
            return 1;
        }
        if (!FileReader::isWriteable(path)
        || !FileReader::isWriteable(fs::path(path).stem())) {
            custom::print_yellow("Can not write to the file "
                                 + path
                                 + " permission denied");
            return 1;
        }
    }
#endif
    std::ofstream dest(path, std::ios::out | std::ios::trunc);
    dest << std::setw(2) << json_obj;
    dest.close();
    return 0;
}

std::string ConverterJSON::findFileRecursive(const std::string &file_name
                                             , const std::string &dir) {
    auto dir_tree = fs::recursive_directory_iterator(dir);
    for (const auto &entry : dir_tree) {
        if (entry.path().filename() == file_name)
            return fs::absolute(entry.path()).string();
    }
    return { };
}

std::string
ConverterJSON::findFile(const std::string &file_name, const std::string &dir) {
    std::regex json_pattern("(\\/|)json(|s)$", std::regex::icase);
    //search in specified dir
    for (const auto &entry : fs::directory_iterator(dir)) {
        auto y = entry.path().filename().string();
        if (entry.path().filename().string() == file_name)
            return fs::absolute(entry.path().string());
    }
    // if file has not found tries to find it in the expected directories
    for(const auto &entry : fs::directory_iterator(dir)) {
        auto x = entry.path().filename().string();
        if (is_directory(entry.path())
        && std::regex_search(entry.path().filename().string(), json_pattern)) {
            auto res = findFile((entry.path() / file_name).string());
            if (!res.empty())
                return res;
        }
    }
    return { };
}

void ConverterJSON::printJson(const json &source) {
    for (auto item = source.begin(); item != source.end(); ++item) {
        if (item->is_structured() && !source.is_array()) {
            if (item->is_array())
                custom::print_green(item.key());
            else
                custom::print_blue(item.key() + ":");
            printJson(item.value());
        } else {
            if (source.is_array()) {
                for (const auto& it : item->items()) {
                    if (!it.key().empty()) {
                        custom::print_cyan("\t" + it.key(), false);
                        std::cout << " :\t" << it.value() << std::endl;
                    } else {
                        std::cout << '\t' << it.value() << std::endl;
                    }
                }
            } else {
                custom::print_magenta(item.key(), false);
                if (item.value().is_boolean()) {
                    std::cout << " :\t";
                    if (item.value())
                        custom::print_green(to_string(item.value()));
                    else
                        custom::print_red(to_string(item.value()));
                } else {
                    std::cout << " :\t" << item.value() << std::endl;
                }
            }
        }
    }
}

bool ConverterJSON::checkConfigProperties(const json &json_file) {

    // throws if the content of the file do not match the conditions

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

    return true; // return true otherwise
}

json ConverterJSON::loadConfigJson(const std::string &path) {
    auto json_file = openJson(path);
    if (json_file.is_null()) {
        throw fs::filesystem_error(
                "Config file is missing"
                , path
                , CONFIG_FILE_NAME
                , std::make_error_code(std::errc::no_such_file_or_directory));
    }
    checkConfigProperties(json_file); // throws if config has invalid properties
    return json_file;
}

bool ConverterJSON::checkRequestsProperties(const json &json_file) {

    // throws if the content of the file do not match the conditions

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

    return true; // return true otherwise
}

json ConverterJSON::loadRequestsJson(const std::string &path) {
    auto json_file = openJson(path);
    if (json_file.is_null()) { // if it could not open file
        throw fs::filesystem_error(
                "Requests file is missing"
                , path
                , REQUESTS_FILE_NAME
                , std::make_error_code(std::errc::no_such_file_or_directory));
    }
    // throws if requests has invalid properties
    checkRequestsProperties(json_file);

    return json_file;
}
