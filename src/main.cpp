#include <iostream>

#include "include/converter_json.h"
#include "include/inverted_index.h"
#include "include/search_server.h"
#include "include/custom_functions.h"

int main(int argc, char **argv) {
    int exit_code = 0;
    try {
        ConverterJSON converter;
        switch (argc) {
            case 2:
                converter = ConverterJSON(argv[1]);
                break;
            case 3:
                converter = ConverterJSON(argv[1], argv[2]);
                break;
            default:
                converter = ConverterJSON(std::filesystem::current_path());
        }
        InvertedIndex db;
        SearchServer srv(db);
        db.updateDocumentBase(converter.getTextDocuments());
        converter.putAnswers(srv.search(converter.getRequests()));
    }
    catch (const std::exception &ex) {
        std::cout << ex.what() << std::endl;
        exit_code = 1;
    }
#if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
    std::cin.get();
#endif
    return exit_code;
}