#include "include/screenwriter.h"

ScreenWriter::ScreenWriter(const std::string &path) : converter(path) {
    auto config = converter.getConfig();
    engine_name = config["config"]["name"];
    engine_version = config["config"]["version"];
    jsons_dir = path;
    indexed_documents = config["files"];
}

void ScreenWriter::startSession() {

}

void ScreenWriter::updateInfo() {

}

void ScreenWriter::updateDB() {

}

void ScreenWriter::showHelp() {

}

void ScreenWriter::showStat() {

}

bool ScreenWriter::checkUpdate() {
    return false;
}

void ScreenWriter::handler(const std::string &cmd) {

}
