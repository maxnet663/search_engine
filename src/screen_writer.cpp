#include "include/screen_writer.h"

#include <iostream>

#include "include/project_constants.h"
#include "include/custom_functions.h"

ScreenWriter::ScreenWriter(const std::string &path)
: converter(path)  , srv(document_base) {
    engine_name = converter.getConfig()["config"]["name"];
    engine_version = converter.getConfig()["config"]["version"];
    indexed_documents = removeInvalid(converter.getConfig()["files"]);
    document_base.updateDocumentBase(indexed_documents);
    last_changes_config =
            last_write_time(converter.getJsonDir() / CONFIG_FILE_NAME);
    last_changes_requests =
            last_write_time(converter.getJsonDir() / REQUESTS_FILE_NAME);
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
    if (last_write_time(converter.getJsonDir() / CONFIG_FILE_NAME)
        == last_changes_config) {
        std::cout << "Indexed documents are up to date" << std::endl;
    } else {
        last_changes_config = last_write_time(
                converter.getJsonDir() / CONFIG_FILE_NAME);
        converter.updateConfig();

        std::vector<std::string> new_indexed_documents
            = removeInvalid(converter.getConfig()["files"]);

        if (indexed_documents != new_indexed_documents) {
            indexed_documents = std::move(new_indexed_documents);
            document_base.updateDocumentBase(indexed_documents);
        }
        std::cout << "Indexed documents updated" << std::endl;
    }
}

void ScreenWriter::updateRequests() {
    if (last_write_time(converter.getJsonDir() / REQUESTS_FILE_NAME)
        == last_changes_requests) {
        std::cout << "Requests are up to date\n";
    } else {
        converter.updateRequests();
        last_changes_requests = last_write_time(
                converter.getJsonDir() / REQUESTS_FILE_NAME);
        std::cout << "Requests updated" << std::endl;
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
    std::cout << engine_name                                      << std::endl
              << "Version: "            << engine_version         << std::endl
              << "Current jsons path: " << converter.getJsonDir() << std::endl
              << (checkUpdate() ?       "Update available"
                                :       "Everything is up to date")<< std::endl;
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
                  { std::cout << doc << std::endl; });
}

bool ScreenWriter::checkUpdate() {
    auto path = converter.getJsonDir();
    return last_write_time(path / CONFIG_FILE_NAME) != last_changes_config
    || last_write_time(path / REQUESTS_FILE_NAME) != last_changes_requests;
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
    std::cout << "Search started, wait please...\n";
    converter.putAnswers(srv.search(converter.getRequests()));
    std::cout << "Search done" << std::endl;
}

void ScreenWriter::showAnswers() {
    auto answers= ConverterJSON::openJson(
            converter.getJsonDir() / ANSWERS_FILE_NAME);

    if (answers == nullptr) {
        answers = ConverterJSON::openJson(
                converter.getJsonDir() / EXCEPTION_ANSWERS_FILE_NAME);
    }

    if (answers == nullptr) {
        std::cout << "Oops, looks like file \"answers.json\" does not exists\n"
                     "The file may have been moved or the search has not yet "
                     "been performed."
                  << std::endl;
        return;
    }

    printAnswers(answers["answers"]);
}

void ScreenWriter::printAnswers(const nlohmann::json &answers) {

    for (auto it = answers.begin(); it != answers.end(); it++) {
        std::cout << it.key() << ":\n";
        if (it.value()["result"] == true) {

            if (it.value().contains("relevance")) {
                std::for_each(it.value()["relevance"].begin()
                                ,it.value()["relevance"].end()
                                , [](const nlohmann::json &ans)
                                { std::cout << ans << std::endl; });
            } else {
                std::cout << "doc id: "      << it.value()["docid"]
                          << "relevance: "   << it.value()["rank"]
                          << std::endl;
            }

        } else {
            std::cout << it.value() << std::endl;
        }
    }
}

void ScreenWriter::exit() {
    std::cin.setstate(std::ios_base::eofbit);
}

std::vector<std::string> ScreenWriter::removeInvalid(
        std::vector<std::string> paths) {
    if (!paths.empty()) {
        auto new_end = std::remove_if(paths.begin()
                                      , paths.end()
                                      , [](const std::string &p)
                                      { return !std::filesystem::exists(p); });
        std::for_each(new_end
                      , paths.end()
                      , [](const std::string &p)
                      { std::cout << p << " does not exist\n"; });

        paths.erase(new_end, paths.end());
    }
    return paths;
}
