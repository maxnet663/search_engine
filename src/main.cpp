#include <iostream>
#include "converter_json.h"
#include <vector>
#include "inverted_index.h"

int main() {
    try {
        std::vector<std::vector<std::pair<int, float>>> vec;
        vec = { { {1, 3.4}, {2, 0.3}}, {}, {{2, 8.5}}};
//        ConverterJSON::putAnswers(vec);
        auto res = ConverterJSON::GetTextDocuments();
        auto res2 = ConverterJSON::GetRequests();
        auto res3 = ConverterJSON::GetResponsesLimit();
        ConverterJSON::putAnswers(vec);
    }
    catch (std::exception &ex) {
        std::cerr << ex.what() << std::endl;
    }

    InvertedIndex idx;
    idx.UpdateDocumentBase(ConverterJSON::GetTextDocuments());
    return 0;
}
