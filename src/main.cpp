#include "converter_json.h"
#include "inverted_index.h"
#include "search_server.h"
#include <iostream>
#include <vector>

int main() {
    InvertedIndex idx;
    idx.UpdateDocumentBase(ConverterJSON::GetTextDocuments());
    SearchServer srv(idx);
    ConverterJSON::putAnswers(srv.search(ConverterJSON::GetRequests()));
    return 0;
}
