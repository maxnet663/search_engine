#ifndef MANGER_H
#define MANGER_H

#include <string>
#include <memory>

#include "include/converter_json.h"
#include "include/inverted_index.h"
#include "include/search_server.h"


class Manager {

    ConverterJSON converter;
    InvertedIndex document_base;

    std::string engine_name;
    std::string engine_version;
    std::filesystem::path jsons_dir;
    std::vector<std::string> indexed_documents;

public:

    Manager(std::string path);

    void startSession();

    void updateInfo();

    void updateDB();

private:

    void showHelp();

    void showStat();

    bool checkUpdate();
};

#endif //MANAGER_H
