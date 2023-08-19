#include "converter_json.h"
#include "inverted_index.h"
#include "search_server.h"
#include <iostream>
#include <vector>

int main() {
    try {
        InvertedIndex idx;
        idx.UpdateDocumentBase(ConverterJSON::GetTextDocuments());
        SearchServer srv(idx);
        ConverterJSON::putAnswers(srv.search(ConverterJSON::GetRequests()));
    }
    catch (std::exception &ex) {
        std::cout << ex.what() << std::endl;
        return -1;
    }
    return 0;
}
