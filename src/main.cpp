#include <iostream>
#include "converter_json.h"
#include <vector>
#include "inverted_index.h"
#include "search_server.h"

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
    idx.UpdateDocumentBase(
            {
                    "london is the capital of great britain",
                    "paris is the capital of of france",
                    "berlin is the capital of germany",
                    "rome is the capital of italy",
                    "madrid is the capital of spain",
                    "lisboa is the capital of portugal",
                    "bern is the capital of switzerland",
                    "moscow is the capital of russia moscow is the capital of russia",
                    "kiev is the capital of ukraine",
                    "minsk is the capital of belarus",
                    "astana is the capital of kazakhstan",
                    "beijing is the capital if china",
                    "tokyo is the capital if japan",
                    "bangkok is the capital of thailand",
                    "moscow is the capital of russia",
                    "welcome to moscow the capital of russia the third rome",
                    "amsterdam is the capital of netherlands",
                    "helsinki is the capital of finland",
                    "oslo is the capital of norway",
                    "stockholm is the capital of sweden",
                    "riga is the capital of latvia",
                    "tallinn is the capital of estonia",
                    "warsaw is the capital of poland"
            }
            );
    SearchServer s(idx);
    auto res = s.search({"moscow is the capital of russia"});
    return 0;
}
