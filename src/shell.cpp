#include "include/shell.h"
#include <regex>

Shell::Shell() : status(Status::Uninitialized), srv(document_base) {
    commands = {
            {"update-db", &Shell::updateDB       },
            {"update-rq", &Shell::updateRequests },
            {"help",      &Shell::showHelp       },
            {"status",    &Shell::showStat       },
            {"find",      &Shell::find           },
            {"print-rq",  &Shell::showRequests   },
            {"print-db",  &Shell::showIndexedDocs},
            {"print-ans", &Shell::printAnswers   },
            {"quit",      &Shell::exit           }
    };
    commands_patterns = {
            { std::regex("^up(date|)-db$")  },
            { std::regex("^up(date|)-rq$")  },
            { std::regex("^h(elp|)$")      },
            { std::regex("^stat(us|)$")    },
            { std::regex("^f(ind|)$")      },
            { std::regex("^p(rint|)-rq$")  },
            { std::regex("^p(rint|)-db$")  },
            { std::regex("^p(rint|)-ans$") },
            { std::regex("^q(uit|)$")      }
    };
}

int Shell::operator()() {
    int command_result = 0;
    if (status == Status::Uninitialized) {
        command_result = initialize();
    }
    if (status == Status::Exit || status == Status::Uninitialized)
        return command_result;
    engine_name = pconverter->getConfig()["config"]["name"];
    engine_version = pconverter->getConfig()["config"]["version"];
    indexed_documents = pconverter->getTextDocuments();
    config_path = pconverter->getConfigPath();
    requests_path = pconverter->getRequestsPath();
    last_changes_config = fs::last_write_time(config_path);
    last_changes_requests = fs::last_write_time(requests_path);
    document_base.updateDocumentBase(indexed_documents);
    showStat();
    while (status != Status::Exit) {
        command_result = handler(cmd);
    }
    return command_result;
}

void Shell::generateJsons() {
    json config;
    config["config"]["name"] = "SearchEngine";
    config["config"]["version"] = "0.1";
    config["config"]["max_responses"] = 5;
    config["files"].push_back("/example/path");
    json requests;
    requests["requests"].push_back("example request");
    ConverterJSON::writeJsonToFile(config
                                   , (fs::current_path()
                                   / CONFIG_FILE_NAME).string());
    ConverterJSON::writeJsonToFile(requests
                                   , (fs::current_path()
                                   / REQUESTS_FILE_NAME).string());
}

conv_ptr Shell::handMakeConverter() {
    auto question = "Indicate how to search for "
                    "configuration json files\n"
                    "Enter: -d <directory/to/search>\n"
                    "or : -p '/path/to/search/config' "
                    "'/path/to/search/requests'\n"
                    "quit or q to exit ";
    std::regex quit_pattern("q(uit)?$", std::regex::icase);
    std::regex dir_pattern("-d [^ ]+$");
    std::regex path_pattern("-p [^ ]+ [^ ]+$");
    cmd.trap(question, {quit_pattern, dir_pattern, path_pattern});
    if (status == Status::Exit)
        return nullptr;
    if (std::regex_match(cmd.str(), quit_pattern)) {
        exit();
        return nullptr;
    }
    if (std::regex_match(cmd.str(), dir_pattern)
    || std::regex_match(cmd.str(), path_pattern)) {
        auto args = cmd.parseArgs();
        auto first_arg = args.front();
        args.pop();
        if (first_arg == "-d") {
            auto dir = args.front();
            args.pop();
            return makeConverter(dir);
        }
        if (first_arg == "-p") {
            auto first_path = args.front();
            args.pop();
            auto second_path = args.front();
            args.pop();
            return makeConverter(first_path, second_path);
        }
    }
    return nullptr;
}

int Shell::initialize() {
    pconverter = makeConverter(fs::current_path());
    if (pconverter) {
        status = Status::Initialized;
    } else {
        std::cout << "Failed to automatically detect json files.\n"
                  << std::endl;
        auto question = "Enter 'man' to specify the path to the "
                        "configuration files manually\n"
                        "or enter 'gen' to generate it automatically";
        std::regex man_pattern("man");
        std::regex gen_pattern("gen");
        std::regex quit_pattern("q(uit)?$", std::regex::icase);
        auto res = cmd.trap(question,
                            {man_pattern, gen_pattern, quit_pattern});
        switch (res) {
            case 0: {
                pconverter = handMakeConverter();
                break;
            }
            case 1: {
                generateJsons();
                auto conf_path = fs::current_path() / CONFIG_FILE_NAME;
                auto req_path =
                        fs::current_path() / REQUESTS_FILE_NAME;
                pconverter = makeConverter(conf_path, req_path);
                break;
            }
            case 2:
                exit();
                return 0;
            default:
                return 1;
        }
        if (pconverter)
            status = Status::Initialized;
    }
    return 0;
}

int Shell::handler(Cmd &command) {
    auto res = cmd.trap("", commands_patterns);
    if (cmd.eof() || res < 0) {
        status = Status::Exit;
        return 1;
    }
    (this->*commands[res].second)();
    return 0;
}

void Shell::updateDB() {
    // check if the file has been edited
    if (fs::last_write_time(pconverter->getConfigPath())
        == last_changes_config) {
        custom::print_green("Indexed documents are up to date");
    } else { // if there are any new updates
        last_changes_config = fs::last_write_time(pconverter->getConfigPath());
        pconverter->updateConfig();

        std::vector<std::string> new_indexed_documents
        = makeAbsolute(pconverter->getConfig()["files"]);

        if (indexed_documents != new_indexed_documents) {
            indexed_documents = std::move(new_indexed_documents);
            document_base.updateDocumentBase(indexed_documents);
        }
        custom::print_green("Indexed documents updated");
    }
}

void Shell::updateRequests() {
    // check if the file has been edited
    if (fs::last_write_time(pconverter->getRequestsPath())
        == last_changes_requests) {
        custom::print_green("Requests are up to date");
    } else { // if there are any new updates
        pconverter->updateRequests();
        last_changes_requests = fs::last_write_time(
                pconverter->getRequestsPath());
        custom::print_green("Requests updated");
    }
}

bool Shell::checkUpdate() {
    return fs::last_write_time(pconverter->getConfigPath())
    != last_changes_config
    || fs::last_write_time(pconverter->getRequestsPath())
    != last_changes_requests;
}

void Shell::showHelp() {
    std::cout <<    "Commands:\n"
       "update-db   : updates data base if you changed indexed docs\n"
       "update-rq   : updates requests if you changed requests.json\n"
       "help        : prints this message\n"
       "status      : prints info about session\n"
       "find        : searches for current queries in the current database\n"
       "print-rq    : prints current requests from requests.json\n"
       "print-db    : prints current indexed docs from config.json\n"
       "print-ans   : prints current search results from answers.json\n"
       "quit        : ends the session" << std::endl;
}

void Shell::showStat() {
    std::cout << engine_name << std::endl
    << "Version: ";
    custom::print_green(engine_version);

    std::cout << "Current config.json: ";
    custom::print_green(pconverter->getConfigPath());

    std::cout << "Current requests.json: ";
    custom::print_green(pconverter->getRequestsPath());

    if (checkUpdate())
        custom::print_green("Update available");
    else
        std::cout << "Everything is up to date\n";
}

void Shell::showRequests() {
    std::cout << "Current Requests:\n";
    auto queries = pconverter->getRequests();
    std::for_each(queries.begin()
                  , queries.end()
                  ,[](const std::string &q)
                  { std::cout << q << std::endl; });
}

void Shell::showIndexedDocs() {
    std::cout << "Current Indexed Documents:\n";
    std::for_each(indexed_documents.begin()
                  , indexed_documents.end()
                  , [](const std::string &doc)
                  { custom::print_green(doc); });
}

void Shell::find() {
    custom::print_green("Search started, please wait...");
    pconverter->putAnswers(srv.search(pconverter->getRequests()));
    custom::print_green("Search done");
}

void Shell::printAnswers() {
    auto answers= ConverterJSON::openJson(ANSWERS_FILE_NAME);

    if (answers == nullptr) { // answers.json does not exist
        std::string msg = "Oops, looks like file \"answers.json\" does not "
                          "exists;\n The file may have been moved or the search"
                          " has not yet been performed.";
        custom::print_yellow(msg);
        return;
    }
    printWithFormatting(answers["answers"]);
}

void Shell::printWithFormatting(const json &answers) {

    for (auto it = answers.begin(); it != answers.end(); it++) {
        custom::print_blue(it.key());

        if (it.value()["result"] == true) { // if there is at least one result

            // lambda for printing json entry
            auto print_entry = [](const nlohmann::json &ans) {
                std::cout << "document id:\t";
                custom::print_green(ans["docid"].dump());
                std::cout << "relevance:  \t";
                custom::print_green(ans["rank"].dump());
            };

            if (it.value().contains("relevance")) { // if there are several
                std::for_each(it.value()["relevance"].begin()
                                ,it.value()["relevance"].end()
                                , print_entry);
            } else { // if there is only one
                std::cout << "document id:\t"   ;
                custom::print_green(it.value()["docid"].dump());
                std::cout << "relevance:\t";
                custom::print_green(it.value()["rank"].dump());
            }
        } else {
            custom::print_red(it.value().dump());
        }
    }
}

std::vector<std::string> Shell::makeAbsolute(
        std::vector<std::string> paths) {
    if (!paths.empty()) {
        // moves invalid paths to the end of the vector
        auto new_end = std::remove_if(paths.begin()
                                      , paths.end()
                                      , [](const std::string &p)
                                      { return !fs::exists(p); });
        // printWithFormatting warnings
        std::for_each(new_end
                      , paths.end()
                      , [](const std::string &p)
                      { custom::print_yellow(p + " does not exist"); });
        paths.erase(new_end, paths.end());

        // change paths to absolute
        std::for_each(paths.begin()
                      , paths.end()
                      , [](std::string &p)
                      { p = absolute(
                              fs::path(p)).string(); });
    }
    return paths;
}

void Shell::exit() {
    status = Status::Exit;
}