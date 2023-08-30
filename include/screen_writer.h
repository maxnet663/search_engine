#ifndef MANGER_H
#define MANGER_H

#include <string>

#include "include/converter_json.h"
#include "include/inverted_index.h"
#include "include/search_server.h"


class ScreenWriter {

    ConverterJSON converter;
    InvertedIndex document_base;

    std::string engine_name;
    std::string engine_version;
    std::filesystem::path jsons_dir;
    std::vector<std::string> indexed_documents;

public:

    ScreenWriter(const std::string &path);

    void startSession();

    void updateInfo();

    void updateDB();

private:

    void showHelp();

    void showStat();

    bool checkUpdate();

    void handler(const std::string &cmd);
};

#endif //MANAGER_H
