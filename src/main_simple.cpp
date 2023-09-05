#include <iostream>

#include "include/converter_json.h"
#include "include/inverted_index.h"
#include "include/search_server.h"
#include "include/custom_functions.h"

int main() {
    ConverterJSON convertor;
    InvertedIndex db;
    db.updateDocumentBase(convertor.getTextDocuments());
    SearchServer srv(db);
    try {
        convertor.putAnswers(srv.search(convertor.getRequests()));
        custom::print_green("Search done");
    }
    catch (const std::exception &ex) {
        std::cerr << ex.what() << std::endl;
    }
#if defined(_WIN32) || defined(WIN32)
    std::cin.get();
#endif
}