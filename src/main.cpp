#include <iostream>
#include "converter_json.h"

int main() {
    try {
        ConverterJSON::GetTextDocuments();
    }
    catch (std::exception &ex) {
        std::cerr << ex.what() << std::endl;
    }
}
