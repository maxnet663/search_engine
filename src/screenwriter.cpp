#include "include/screenwriter.h"

Manager::Manager(std::string path) : converter(path) {
    auto config = converter.getConfig();
    engine_name = config["config"]["name"];
    engine_version = config["config"]["version"];
    jsons_dir = path;
    indexed_documents = config["files"];
}