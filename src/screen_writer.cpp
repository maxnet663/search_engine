#include "include/screen_writer.h"

#include <iostream>

#include "include/project_constants.h"

ScreenWriter::ScreenWriter(const std::string &path)
: converter(path)  , srv(document_base) {

    engine_name = converter.getConfig()["config"]["name"];
    engine_version = converter.getConfig()["config"]["version"];
    indexed_documents = makeAbsolute(converter.getConfig()["files"]);
    document_base.updateDocumentBase(indexed_documents);
    last_changes_config =
            std::filesystem::last_write_time(converter.getConfigPath());
    last_changes_requests =
            std::filesystem::last_write_time(converter.getRequestsPath());
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

void ScreenWriter::startSession() {
    showStat();
    std::string cmd;
    cmd.reserve(16); // any command will not exceed this length
    std::cout << "> ";
    while(!std::getline(std::cin, cmd).eof()) {
        handler(cmd);
    }
}

void ScreenWriter::updateDB() {
    if (std::filesystem::last_write_time(converter.getConfigPath())
        == last_changes_config) {
        custom::print_green("Indexed documents are up to date");
    } else {
        last_changes_config = std::filesystem::last_write_time(
                converter.getConfigPath());
        converter.updateConfig();

        std::vector<std::string> new_indexed_documents
            = makeAbsolute(converter.getConfig()["files"]);

        if (indexed_documents != new_indexed_documents) {
            indexed_documents = std::move(new_indexed_documents);
            document_base.updateDocumentBase(indexed_documents);
        }
        custom::print_green("Indexed documents updated");
    }
}

void ScreenWriter::updateRequests() {
    if (std::filesystem::last_write_time(converter.getRequestsPath())
        == last_changes_requests) {
        custom::print_green("Requests are up to date");
    } else {
        converter.updateRequests();
        last_changes_requests = std::filesystem::last_write_time(
                converter.getRequestsPath());
        custom::print_green("Requests updated");
    }
}

void ScreenWriter::showHelp() {
    std::cout <<    "Commands:\n"
       "update-db   : updates data base if you changed indexed docs\n"
       "update-rq   : updates requests if you changed requests.json\n"
       "help        : prints the list of commands\n"
       "status      : prints info about session\n"
       "find        : searches for current queries in the current database\n"
       "print-rq    : prints current requests from requests.json\n"
       "print-db    : prints current indexed docs from config.json[files]\n"
       "print-ans   : prints cuurent search results from answers.json\n"
       "quit        : ends the session" << std::endl;
}

void ScreenWriter::showStat() {
    std::cout << engine_name << std::endl
    << "Version: ";
    custom::print_green(engine_version);

    std::cout << "Current config.json: ";
    custom::print_green(converter.getConfigPath());

    std::cout << "Current requests.json ";
    custom::print_green(converter.getRequestsPath());

    if (checkUpdate())
        custom::print_green("Update available");
    else
        std::cout << "Everything is up to date\n";
}

void ScreenWriter::showRequests() {
    std::cout << "Current Requests:\n";
    auto queries = converter.getRequests();
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

bool ScreenWriter::checkUpdate() {
    auto conf_path = converter.getConfigPath();
    auto req_path = converter.getRequestsPath();
    return std::filesystem::last_write_time(conf_path) != last_changes_config
      || std::filesystem::last_write_time(req_path) != last_changes_requests;
}

void ScreenWriter::handler(const std::string &cmd) {
    if (!cmd.empty()) {

        if (cmd == "q")
            const_cast<std::string &>(cmd) = "quit";

        auto execute = commands.find(cmd);

        if (execute == commands.end())
            std::cout << "No such a command \"" << cmd << "\"" << std::endl;
        else
            (this->*execute->second)();

    }
    if (!std::cin.eof())
        std::cout << "> ";
}

void ScreenWriter::search() {
    custom::print_green("Search started, please wait...");
    converter.putAnswers(srv.search(converter.getRequests()));
    custom::print_green("Search done");
}

void ScreenWriter::showAnswers() {
    auto answers= ConverterJSON::openJson(ANSWERS_FILE_NAME);

    if (answers == nullptr) {
        answers = ConverterJSON::openJson(EXCEPTION_ANSWERS_FILE_NAME);
    }

    if (answers == nullptr) {
        std::string msg = "Oops, looks like file \"answers.json\" does not "
                          "exists;\n The file may have been moved or the search"
                          " has not yet been performed.";
        custom::print_yellow(msg);
        return;
    }

    printAnswers(answers["answers"]);
}

void ScreenWriter::printAnswers(const nlohmann::json &answers) {

    for (auto it = answers.begin(); it != answers.end(); it++) {
        custom::print_blue(it.key());

        // if there is at least one result
        if (it.value()["result"] == true) {

            // lamda for printing json entry
            auto print_entry = [](const nlohmann::json &ans) {
                std::cout << "document id:\t";
                custom::print_green(ans["docid"].dump());
                std::cout << "relevance:\t";
                custom::print_green(ans["rank"].dump());
            };

            // if there are several
            if (it.value().contains("relevance")) {
                std::for_each(it.value()["relevance"].begin()
                                ,it.value()["relevance"].end()
                                , print_entry);
            } else {
                // if there is only one
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

void ScreenWriter::exit() {
    std::cin.setstate(std::ios_base::eofbit);
}

std::vector<std::string> ScreenWriter::makeAbsolute(
        std::vector<std::string> paths) {
    if (!paths.empty()) {
        auto new_end = std::remove_if(paths.begin()
                                      , paths.end()
                                      , [](const std::string &p)
                                      { return !std::filesystem::exists(p); });
        std::for_each(new_end
                      , paths.end()
                      , [](const std::string &p)
                      { custom::print_yellow(p + " does not exist"); });

        paths.erase(new_end, paths.end());

        std::for_each(paths.begin()
                      , paths.end()
                      , [](std::string &p)
                      { p = absolute(
                              std::filesystem::path(p)).string(); });
    }
    return paths;
}
