#include "converter_json.h"
#include "file_helper.h"
#include "file_helper.cpp"
#include "converter_json.cpp"
#include "gtest/gtest.h"

TEST(ConverterJSONTest, EmptyDocumentsListTest) {
    std::vector documents = ConverterJSON::GetTextDocuments();
    ASSERT_TRUE(documents.empty());
}


int test_main() {

    return 0;
}