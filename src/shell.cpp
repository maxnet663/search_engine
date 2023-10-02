#include "include/screen_writer.h"

ScreenWriter::ScreenWriter(std::queue<std::string> args) : srv(document_base) {
    auto flag = args.front();
    args.pop();
    if (flag == "-p") {
        auto first = args.front();
        args.pop();
        auto second = args.front();
        args.pop();
        pconverter = makeConverter(first, second);
    } else if (flag == "-d") {
        pconverter = makeConverter(args.front());
    } else {
        pconverter = makeConverter(std::filesystem::current_path().string());
    }
    if (!pconverter) {
        throw std::runtime_error("Program stopped");
    }
    engine_name = pconverter->getConfig()["config"]["name"];
    engine_version = pconverter->getConfig()["config"]["version"];
    indexed_documents = makeAbsolute(pconverter->getConfig()["files"]);
    document_base.updateDocumentBase(indexed_documents);
    last_changes_config =
            std::filesystem::last_write_time(pconverter->getConfigPath());
    last_changes_requests =
            std::filesystem::last_write_time(pconverter->getRequestsPath());
    document_base.updateDocumentBase(indexed_documents);
    commands = {
            {"update-db", &ScreenWriter::updateDB       },
            {"update-rq", &ScreenWriter::updateRequests },
            {"help",      &ScreenWriter::showHelp       },
            {"status",    &ScreenWriter::showStat       },
            {"find",      &ScreenWriter::search         },
            {"print-rq",  &ScreenWriter::showRequests   },
            {"print-db",  &ScreenWriter::showIndexedDocs},
            {"print-ans", &ScreenWriter::showAnswers    },
            {"quit",      &ScreenWriter::exit           }
    };
}

void ScreenWriter::operator()() {
    showStat();
    std::string cmd;
    PRINT_INVITATION
    while(!std::getline(std::cin, cmd).eof()) { // start dialog
        handler(cmd);
    }
}

conv_ptr ScreenWriter::handMakeConverter() {
    std::cout << "Indicate how to search for configuration json files\n"
                 "Enter: -d <directory/to/search>\n"
                 "or enter: -p <path/to/search/config> "
                 "</path/to/search/requests>\n"
                 "Enter quit or q to exit\n> ";
    std::string input;
    // start dialog and wait for command
    while (!std::getline(std::cin, input).eof()) {
        // format input
        format::utf::deleteExtraSpaces(input);
        format::utf::toLowerCase(input);
        if (input.empty()) {
            custom::print_yellow("Arguments required");
            PRINT_INVITATION
            continue;
        }
        // parse command and try to execute
        auto args = commandParser(input);
        auto flag = args.front();
        args.pop();
        if (flag == "-d" && !args.empty()) {
            try {
                auto p_converter = std::make_unique<ConverterJSON>(args.front());
                return p_converter;
            }
            catch (std::exception &ex) {
                // if it was not possible to construct the object,
                // then we handle the exception and try again
                custom::print_red(ex.what());
                std::cout << "Try again" << std::endl;
                PRINT_INVITATION
                continue;
            }
        }
        if (flag == "-p" && args.size() >= 2) {
            auto cnf_path = args.front();
            args.pop();
            auto req_path = args.front();
            args.pop();
            try {
                auto p_converter = std::make_unique<ConverterJSON>(cnf_path
                        , req_path);
                return p_converter;
            }
            catch(std::exception &ex) {
                // if it was not possible to construct the object,
                // then we handle the exception and try again
                custom::print_red(ex.what());
                PRINT_INVITATION
                continue;
            }
        }
        if (flag == "quit" || flag == "q") { // exit
            return nullptr;
        }
        custom::print_yellow("Wrong input. Try again"); // handle wrong input
        PRINT_INVITATION
    }
    return nullptr;
}

std::queue<std::string> ScreenWriter::commandParser(const std::string &cmd) {
    std::stringstream line(cmd);
    std::string buf;
    std::queue<std::string> args;
    while(line >> buf) {
        args.push(buf);
    }
    return args;
}

void ScreenWriter::handler(std::string &cmd) {
    // format command
    format::utf::deleteExtraSpaces(cmd);
    format::utf::toLowerCase(cmd);
    if (!cmd.empty()) {

        if (cmd == "q")
            cmd = "quit";

        // search for a command
        auto execute = commands.find(cmd);

        if (execute == commands.end()) // if there is no such a command
            std::cout << "No such a command \"" << cmd << "\"" << std::endl;
        else // if cmd is found - execute
            (this->*execute->second)();
    }
    if (!std::cin.eof()) // handle ^D (eof situation)
        PRINT_INVITATION
}

void ScreenWriter::updateDB() {
    // check if the file has been edited
    if (std::filesystem::last_write_time(pconverter->getConfigPath())
        == last_changes_config) {
        custom::print_green("Indexed documents are up to date");
    } else { // if there are any new updates
        last_changes_config = std::filesystem::last_write_time(
                pconverter->getConfigPath());
        pconverter->updateConfig();

        std::vector<std::string> new_indexed_documents = makeAbsolute(
                pconverter->getConfig()["files"]);

        if (indexed_documents != new_indexed_documents) {
            indexed_documents = std::move(new_indexed_documents);
            document_base.updateDocumentBase(indexed_documents);
        }
        custom::print_green("Indexed documents updated");
    }
}

void ScreenWriter::updateRequests() {
    // check if the file has been edited
    if (std::filesystem::last_write_time(pconverter->getRequestsPath())
        == last_changes_requests) {
        custom::print_green("Requests are up to date");
    } else { // if there are any new updates
        pconverter->updateRequests();
        last_changes_requests = std::filesystem::last_write_time(
                pconverter->getRequestsPath());
        custom::print_green("Requests updated");
    }
}

bool ScreenWriter::checkUpdate() {
    auto conf_path = pconverter->getConfigPath();
    auto req_path = pconverter->getRequestsPath();
    return std::filesystem::last_write_time(pconverter->getConfigPath())
    != last_changes_config
    || std::filesystem::last_write_time(pconverter->getRequestsPath())
    != last_changes_requests;
}

void ScreenWriter::showHelp() {
    std::cout <<    "Commands:\n"
       "update-db   : updates data base if you changed indexed docs\n"
       "update-rq   : updates requests if you changed requests.json\n"
       "help        : prints the list of commands\n"
       "status      : prints info about session\n"
       "find        : searches for current queries in the current database\n"
       "print-rq    : prints current requests from requests.json\n"
       "print-db    : prints current indexed docs from config.json\n"
       "print-ans   : prints cuurent search results from answers.json\n"
       "quit        : ends the session" << std::endl;
}

void ScreenWriter::showStat() {
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

void ScreenWriter::showRequests() {
    std::cout << "Current Requests:\n";
    auto queries = pconverter->getRequests();
    std::for_each(queries.begin()
                  , queries.end()
                  ,[](const std::string &q)
                  { std::cout << q << std::endl; });
}

void ScreenWriter::showIndexedDocs() {
    std::cout << "Current Indexed Documents:\n";
    std::for_each(indexed_documents.begin()
                  , indexed_documents.end()
                  , [](const std::string &doc)
                  { custom::print_green(doc); });
}

void ScreenWriter::search() {
    custom::print_green("Search started, please wait...");
    pconverter->putAnswers(srv.search(pconverter->getRequests()));
    custom::print_green("Search done");
}

void ScreenWriter::showAnswers() {
    auto answers= ConverterJSON::openJson(ANSWERS_FILE_NAME);

    if (answers == nullptr) { // answers.json does not exist
        std::string msg = "Oops, looks like file \"answers.json\" does not "
                          "exists;\n The file may have been moved or the search"
                          " has not yet been performed.";
        custom::print_yellow(msg);
        return;
    }
    printAnswers(answers["answers"]);
}

void ScreenWriter::printAnswers(const json &answers) {

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

std::vector<std::string> ScreenWriter::makeAbsolute(
        std::vector<std::string> paths) {
    if (!paths.empty()) {
        // moves invalid paths to the end of the vector
        auto new_end = std::remove_if(paths.begin()
                                      , paths.end()
                                      , [](const std::string &p)
                                      { return !std::filesystem::exists(p); });
        // print warnings
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
                              std::filesystem::path(p)).string(); });
    }
    return paths;
}

void ScreenWriter::exit() {
    std::cin.setstate(std::ios_base::eofbit); // imitate eof situation
}