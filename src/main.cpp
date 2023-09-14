#include <iostream>

#include "include/converter_json.h"
#include "include/inverted_index.h"
#include "include/search_server.h"
#include "include/custom_functions.h"
#include "include/screen_writer.h"

int main() {
    try {
        auto converter = ScreenWriter::makeConverter(
                std::filesystem::current_path().string());
        if (!converter) {
            custom::print_red("Search canceled.");
            return 1;
        }
        InvertedIndex db;
        db.updateDocumentBase(converter->getTextDocuments());
        SearchServer srv(db);
        converter->putAnswers(srv.search(converter->getRequests()));
        custom::print_green("Search done");
    }
    catch (const std::exception &ex) {
        custom::print_red(ex.what());
    }
#if defined(_WIN32) || defined(WIN32)
    std::cin.get();
#endif
    return 0;
}