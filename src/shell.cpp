#include "include/shell.h"

#include <regex> // regex regex_match

#include "include/file_reader.h"

Shell::Shell(int argc, char **argv) : cmd(argc, argv) {
    commands = {
            { std::regex("^gen(erate|)$") , &Shell::generateJsons },
            { std::regex("^h(elp|)$")     , &Shell::showHelp      },
            { std::regex("^conf(ig|)$")   , &Shell::showConfig    },
            { std::regex("^f(ind|)$")     , &Shell::find          },
            { std::regex("^(index|idx)$") , &Shell::index         },
            { std::regex("^p(rint|)$")    , &Shell::printJson     }

    };
    auto helper_path = ConverterJSON::findFile("help.json");
    if (helper_path.empty()) {
        custom::print_yellow("Could not find helper file. "
                             "Did you do something to him?");
    } else {
        helper = ConverterJSON::openJson(helper_path);
    }
}

int Shell::operator()() {
    auto args = cmd.parseArgs();
    auto first = getNext(args);
    for (auto &command : commands) {
        if (std::regex_match(first, command.first)) {
            auto exec = command.second;
            return (exec)(this, args);
        }
    }
    std::cout << "Unknown command " + first << std::endl;
    return 0;
}

int Shell::showHelp(std::queue<std::string> &args) {
    if (args.empty()) {
        std::cout <<
        "General commands:\n"
        "[generate] or [gen]  : generate default json files\n"
        "[help] or [h]        : prints this message. Use -all to see more\n"
        "[config] or [conf]   : print info about this program\n"
        "[find] or [f]        : searches for current queries in the "
                                "current database\n"
        "[index] or [idx]     : recursive searches for txt files in directory "
                                "and adds them to indexed files\n"
        "[print] or [p]       : print out the content of json file in standard"
                                "output\n"
        << std::endl;
    } else if (args.front() == "-all") {
        help();
    } else {
        custom::print_yellow("Unknown argument " + args.front());
        return 1;
    }
    return 0;
}

int Shell::help(const std::string& command) {
    if (command.empty()) {
        ConverterJSON::printJson(helper);
    } else {
        if (!helper.contains(command)) {
            custom::print_yellow("Unknown command");
            return 1;
        } else {
            std::cout << command << std::endl;
            ConverterJSON::printJson(helper[command]);
        }
    }
    return 0;
}

int Shell::generateJsons(args_t &args) {
    std::string dest_dir = fs::current_path().string();
    bool explicit_path = false;
    while (!args.empty()) {
        auto arg = getNext(args);
        if (arg == "-h") {
            help("generate");
            return 0;
        }
        if (!explicit_path) {
            dest_dir = arg;
            explicit_path = true;
            continue;
        } else {
            custom::print_yellow("Unknown argument " + arg);
            return 1;
        }
    }
    return generate(CONFIG_FILE_NAME, REQUESTS_FILE_NAME, dest_dir);
}

int Shell::generate(const std::string &config_name,
                    const std::string &requests_name,
                    const std::string &dest_dir) {
    json config;
    config["config"]["name"] = "SearchEngine";
    config["config"]["version"] = "0.1";
    config["config"]["max_responses"] = 5;
    config["files"].push_back("/example/path");
    json requests;
    requests["requests"].push_back("example request");
    fs::create_directories(dest_dir);
    if (!FileReader::isWriteable(dest_dir)) {
        custom::print_yellow("Permission denied " + dest_dir);
        return 1;
    }
    auto res = ConverterJSON::writeJsonToFile(config
            , (fs::path(dest_dir)
            / config_name).string());
    if (!res)
        custom::print_green(config_name + " generated in " + dest_dir);
    res = ConverterJSON::writeJsonToFile(requests
            , (fs::path(dest_dir) / requests_name).string());
    if (!res)
        custom::print_green(requests_name + " generated in " + dest_dir);
    return 0;
}

int Shell::showConfig(std::queue<std::string> &args) {
    std::string config_path = CONFIG_FILE_NAME;
    bool recursive_search = false;
    bool explicit_path = false;
    while (!args.empty()) {
        auto arg = getNext(args);
        if (arg == "-h") {
            help("config");
            return 0;
        }
        if (arg == "-r") { // ignored if path indicated explicitly
            recursive_search = true;
        } else {
            explicit_path = true;
            config_path = arg;
        }
    }
    json config;
    if (!explicit_path) {
        if (recursive_search)
            config_path = ConverterJSON::findFileRecursive(config_path);
        else
            config_path = ConverterJSON::findFile(config_path);

    }
    if (config_path.empty()) {
        custom::print_yellow("Could not find " CONFIG_FILE_NAME);
        return 1;
    }
    config = ConverterJSON::openJson(config_path);
    if(config.empty())
        return 1;
    printInfo(config);
    return 0;
}

void Shell::printInfo(const json &config) {
    std::cout << "Engine name: " << config["config"]["name"] << std::endl;
    std::cout << "Engine version: " << config["config"]["version"] << std::endl;
    printIndexingPath(config["files"]);
}

void Shell::printIndexingPath(const std::vector<std::string> &path) {
    for (const auto &p : path) {
        if (!fs::exists(p))
            custom::print_yellow("File " + p + " does not exist");
        else
            custom::print_green(p + " ok");
    }
}

int Shell::find(args_t &args) {
    std::string config_path = CONFIG_FILE_NAME;
    std::string requests_path = REQUESTS_FILE_NAME;
    bool recursive_search = false;
    bool explicit_path = false;
    bool print_answers = false;
    while (!args.empty()) {
        auto arg = getNext(args);
        if (arg == "-h") {
            help("find");
            return 0;
        }
        if (arg == "-print") {
            if (!print_answers) {
                print_answers = true;
            } else {
                custom::print_yellow("Extra -print arg?");
                return 1;
            }
            continue;
        }
        if (arg == "-r") {
            recursive_search = true;
            continue;
        }
        if (!explicit_path) {
            explicit_path = true;
            config_path = arg;
            requests_path = getNext(args);
            if (config_path.empty() || requests_path.empty()) {
                custom::print_yellow("Expected paths to config and requests");
                return 1;
            }
            continue;
        }
        custom::print_yellow("Unknown argument " + arg);
        return 1;
    }
    auto res = makeSearch(config_path
               , requests_path
               , explicit_path
               , recursive_search);
    if (!res && print_answers)
        ConverterJSON::printJson(ConverterJSON::openJson(ANSWERS_FILE_NAME));
    return res;
}

int Shell::makeSearch(std::string &config_path
                      , std::string &requests_path
                      , bool explicit_flag
                      , bool recursive_flag) {
    conv_ptr pconverter;
    if (explicit_flag) {
        pconverter = makeConverter(config_path, requests_path);
    } else {
        if (recursive_flag) {
            config_path = ConverterJSON::findFileRecursive(config_path);
            requests_path = ConverterJSON::findFileRecursive(requests_path);
            pconverter = makeConverter(config_path, requests_path);
        } else {
            pconverter = makeConverter(fs::current_path());
        }
    }
    if (!pconverter)
        return 1;
    InvertedIndex idx;
    idx.updateDocumentBase(pconverter->getDocumentsPaths());
    SearchServer srv(idx);
    pconverter->putAnswers(srv.search(pconverter->getRequests()));
    return 0;
}

int Shell::index(std::queue<std::string> &args) {
    if (args.empty()) {
        custom::print_yellow("Arguments required. Use -h to get help");
        return 1;
    }
    std::string dir_to_indexing;
    std::string config_path = CONFIG_FILE_NAME;
    bool recursive = false;
    while (!args.empty()) {
        auto arg = getNext(args);
        if (arg == "-h") {
            help("index");
            return 0;
        }
        if (arg == "-r") {
            recursive = true;
            continue;
        }
        if (dir_to_indexing.empty()) {
            dir_to_indexing = arg;
            continue;
        }
        config_path = arg;
        if (!arg.empty())
            config_path = arg;
    }
    if (dir_to_indexing.empty()) {
        custom::print_yellow("Expected path to indexing docs");
        return 1;
    }
    if (recursive)
        config_path = ConverterJSON::findFileRecursive(config_path);
    else
        config_path = ConverterJSON::findFile(config_path);
    if (config_path.empty()) {
        custom::print_yellow("Could not find config.json");
        return 1;
    }
    json config = ConverterJSON::openJson(config_path);
    if (config.empty())
        return 1;
    auto res = addFiles(dir_to_indexing, config);
    ConverterJSON::writeJsonToFile(config, config_path);
    if (!res)
        custom::print_green("Indexed files has written to " + config_path);
    else
        custom::print_yellow("Something went wrong during indexing");
    return res;
}

int Shell::addFiles(const std::string &dir, json &config_json) {
    if (!fs::is_directory(dir)) {
        custom::print_yellow(dir + " is not a directory");
        return 1;
    }
    std::vector<std::string> files;
    for (const auto &entry : fs::recursive_directory_iterator(dir)) {
        if (fs::is_regular_file(entry) && entry.path().extension() == ".txt")
            files.emplace_back(entry.path().string());
    }
    for (auto &file : files) {
        config_json["files"].push_back(file);
    }
    return 0;
}

int Shell::printJson(std::queue<std::string> &args) {
    if (args.empty()) {
        custom::print_yellow("Path to file required");
        return 1;
    }
    auto arg = getNext(args);
    if (arg == "-h") {
        return help("print");
    }
    std::regex json_pattern("[^ ]+\\.json$");
    if (!std::regex_match(arg, json_pattern)) {
        custom::print_yellow("Strange argument. Expected path to *.json");
        return 1;
    }
    auto obj = ConverterJSON::openJson(arg);
    if (obj.empty()) {
        return 1;
    } else {
            ConverterJSON::printJson(obj);
    }
    return 0;
}

std::string Shell::getNext(args_t &args) {
    if (args.empty()) {
        return { };
    } else {
        auto front = args.front();
        args.pop();
        return front;
    }
}
