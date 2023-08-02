#include <iostream>
#include "converter_json.h"

int main() {
    try {
        std::vector<std::vector<std::pair<int, float>>> vec;
        vec = { { {1, 3.4}, {2, 0.3}}, {}, {{2, 8.5}}};
        ConverterJSON::putAnswers(vec);
    }
    catch (std::exception &ex) {
        std::cerr << ex.what() << std::endl;
    }
}
