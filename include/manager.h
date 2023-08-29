#ifndef MANGER_H
#define MANGER_H

#include <string>
#include <memory>

#include "include/converter_json.h"
#include "include/inverted_index.h"
#include "include/search_server.h"


class Manager {

   std::string engine_name;
   std::string engine_version;
   std::filesystem::path jsons_dir;
   std::filesystem::path resources_dir;
   std::vector<std::string> indexed_documents;

   std::unique_ptr<ConverterJSON> converter;
   std::shared_ptr<InvertedIndex> document_base;
   std::unique_ptr<SearchServer> searchServer;

public:

    Manager();

};

#endif //MANAGER_H
