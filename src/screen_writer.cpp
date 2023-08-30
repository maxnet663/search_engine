#include "include/screen_writer.h"

#include <iostream>

#include "include/project_constants.h"

ScreenWriter::ScreenWriter(const std::string &path) : converter(path)
                                                    , srv(document_base) {
    engine_name = converter.getConfig()["config"]["name"];
    engine_version = converter.getConfig()["config"]["version"];
    indexed_documents = converter.getConfig()["files"];
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
            {"quit",      &ScreenWriter::exit           }
    };
}

void ScreenWriter::startSession() {
    showStat();
    std::string cmd;
    cmd.reserve(16);
    std::cout << "> ";
    while(!std::getline(std::cin, cmd).eof()) {
        handler(cmd);
    }
    std::cin.setstate(std::ios_base::goodbit);
}

void ScreenWriter::updateDB() {
    if (last_write_time(converter.getJsonDir() / CONFIG_FILE_NAME)
        == last_changes_config)
        return;

    last_changes_config =
            last_write_time(converter.getJsonDir() / CONFIG_FILE_NAME);
    converter.updateConfig();
    auto new_indexed_documents = converter.getConfig()["files"];
    if (has_diff(indexed_documents, new_indexed_documents)) {
        indexed_documents = std::move(new_indexed_documents);
        document_base.updateDocumentBase(indexed_documents);
    }
    std::cout << "Indexed documents updated" << std::endl;
}

void ScreenWriter::updateRequests() {
    if (last_write_time(converter.getJsonDir() / REQUESTS_FILE_NAME)
        == last_changes_requests) {
        std::cout << "Requests are up to date\n";
        return;
    }

    converter.updateRequests();
    last_changes_requests =
            last_write_time(converter.getJsonDir() / REQUESTS_FILE_NAME);
    std::cout << "Requests updated" << std::endl;
}

void ScreenWriter::showHelp() {
    std::cout << "Commands:" << std::endl
    << "update-db   : updates data base if you changed indexed docs\n"
    << "update-rq   : updates requests if you changed requests.json\n"
    << "help        : prints the list of commands\n"
    << "status      : prints info about session\n"
    << "find        : searches for current queries in the current database\n"
    << "quit        : ends the session" << std::endl;
}

void ScreenWriter::showStat() {
    std::cout << engine_name << std::endl
    << "Version: " << engine_version << std::endl
    << "Current JSONs path: " << converter.getJsonDir() << std::endl
    << (checkUpdate() ? "Update available" : "Everything is up to date")
    << std::endl;
}

bool ScreenWriter::checkUpdate() {
    auto path = converter.getJsonDir();
    auto x = last_write_time(path / CONFIG_FILE_NAME);
    auto y = last_write_time(path / REQUESTS_FILE_NAME);
    return  x != last_changes_config
        || y != last_changes_requests;
}

void ScreenWriter::handler(const std::string &cmd) {
    if (!cmd.empty()) {
        if (cmd == "q")
            const_cast<std::string &>(cmd) = "quit";
        auto execute = commands.find(cmd);
        if (execute == commands.end()) {
            std::cout << "No such a command \"" << cmd << "\"" << std::endl;
        } else {
            (this->*execute->second)();
        }
    }
    std::cout << "> ";
}

bool ScreenWriter::has_diff(const std::vector<std::string> &left
                            , const std::vector<std::string> &right) {
    return left != right;
}

void ScreenWriter::search() {
    converter.putAnswers(srv.search(converter.getRequests()));
}

void ScreenWriter::exit() {
    std::cin.setstate(std::ios_base::eofbit);
}
