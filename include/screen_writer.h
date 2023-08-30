#ifndef MANGER_H
#define MANGER_H

#include <string>
#include <map>

#include "include/converter_json.h"
#include "include/inverted_index.h"
#include "include/search_server.h"


class ScreenWriter {

    ConverterJSON converter;
    InvertedIndex document_base;
    SearchServer srv;

    std::string engine_name;
    std::string engine_version;
    std::vector<std::string> indexed_documents;
    std::filesystem::file_time_type last_changes_config;
    std::filesystem::file_time_type last_changes_requests;

    std::map<std::string, void(ScreenWriter::*)()> commands;

public:

    ScreenWriter(const std::string &path);

    void startSession();

private:

    void updateDB();

    void updateRequests();

    bool checkUpdate();

    void showHelp();

    void showStat();

    void handler(const std::string &cmd);

    bool has_diff(const std::vector<std::string> &left
                  , const std::vector<std::string> &right);

    void search();

    void exit();
};

#endif //MANAGER_H
