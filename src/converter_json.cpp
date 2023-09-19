#include "include/converter_json.h"

#include <fstream> // ifs, ofs
#include <iostream> // cerr

#include "include/custom_functions.h"
#include "include/project_constants.h"

ConverterJSON::ConverterJSON(const PathType &jsons_dir) {
    if (!std::filesystem::is_directory(jsons_dir)) {
        throw std::filesystem::filesystem_error(
                "Wrong path"
                , jsons_dir
                ,std::make_error_code(std::errc::not_a_directory));
    }

    // try to find paths to json files
    config_path = findFile(CONFIG_FILE_NAME, jsons_dir );
    if (config_path.empty()) {
        throw std::invalid_argument("Could not find " CONFIG_FILE_NAME);
    }
    custom::print_green("config.json found successfully");

    requests_path = findFile(REQUESTS_FILE_NAME, jsons_dir);
    if (requests_path.empty()) {
        throw std::invalid_argument("Could not find " REQUESTS_FILE_NAME);
    }
    custom::print_green("requests.json found successfully");

    config = loadConfigJson(config_path);
    requests = loadRequestsJson(requests_path);
}

ConverterJSON::ConverterJSON(PathType conf_p, PathType req_p)
: config_path(std::move(conf_p)), requests_path(std::move(req_p)) {
    config = loadConfigJson(config_path);
    custom::print_green("config.json found successfully");
    requests = loadRequestsJson(requests_path);
    custom::print_green("requests.json found successfully");
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

    for (const auto &i : requests["requests"]) {
        std::string buf = to_string(i); // get request
        custom::formatString(buf); // format it
        requests_list.push_back(std::move(buf)); // write to requests_list
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
    json json_file;

    // get round all the elements of the answers
    // and write them down in the final structure
    for (size_t i = 0; i < answers.size(); ++i) {
        std::string request = "request";

        // form number of the request
        std::string number(std::to_string(i + 1)); // 1 <= number <= 1000
        // fill spaces by zeroes
        number.insert(0, std::string(4 - number.length(), '0'));
        request.append(number); // get "requestNNNN"

        bool search_result = !answers.empty();
        // if nothing is found for this request
        if (search_result) {
            json_file["answers"][request]["result"] = search_result;

        } else {

            //if only one answer is found
            if (answers[i].size() == 1) {
                json_file["answers"][request]["result"] = search_result;
                json_file["answers"][request]["docid"] = answers[i][0].doc_id;
                json_file["answers"][request]["rank"] = answers[i][0].rank;

            } else {

                // if multiple results are found, add a field relevance
                json_file["answers"][request]["result"] = search_result;

                auto limit = static_cast<size_t>(getResponsesLimit());
                for (size_t j = 0; j < limit && j < answers[i].size(); ++j) {
                    // element of relevance array
                    json field;
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
        custom::writeJsonToFile(json_file, RESERVE_ANSWERS_FILE_NAME);
        custom::print_green((std::filesystem::current_path()
                                    / RESERVE_ANSWERS_FILE_NAME).string());
        custom::print_yellow("Could not write to the file \"answers.json\"\n"
                             "Result have written to \"answers_safe.json\"\n");
    }
}

void ConverterJSON::updateConfig(const PathType &path) {
    try {
        if (path.empty())
            config = loadConfigJson(config_path);
        else
            config = loadConfigJson(path);
   }
    catch (std::exception &ex) {
        custom::print_yellow(ex.what());
        config = nullptr;
    }
}

void ConverterJSON::updateRequests(const PathType &path) {
    try {
        if (path.empty())
            requests = loadRequestsJson(requests_path);
        else
            requests = loadRequestsJson(path);
    }
    catch (std::exception &ex) {
        custom::print_yellow(ex.what());
        config = nullptr;
    }
}

json ConverterJSON::openJson(const PathType &path) {
    if (!std::filesystem::exists(path) || !custom::isReadable(path)) {
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
