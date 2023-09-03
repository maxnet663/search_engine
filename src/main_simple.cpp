#include <iostream>

#include "include/converter_json.h"
#include "include/inverted_index.h"
#include "include/search_server.h"

int main() {
    ConverterJSON convertor(std::filesystem::current_path() / "jsons");
    InvertedIndex db;
    db.updateDocumentBase(convertor.getTextDocuments());
    SearchServer srv(db);
    try {
        convertor.putAnswers(srv.search(convertor.getRequests()));
        std::cout << "Search done" << std::endl;
    }
    catch (const std::exception &ex) {
        std::cerr << ex.what() << std::endl;
    }
}