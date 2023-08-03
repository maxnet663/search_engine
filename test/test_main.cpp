//#include "file_helper.h"
#include "file_helper.cpp"
//#include "converter_json.h"
#include "converter_json.cpp"
#include "gtest/gtest.h"

TEST(ConverterJSONTest, EmptyDocumentsListTest) {
    auto documents = ConverterJSON::GetTextDocuments();
    ASSERT_TRUE(!documents.empty());
}

TEST(ConverterJSONTest, GetTextDocumentsTest) {
    std::vector<std::string> expected_result = {
            "milk water salt",
            "milk apple banana",
            "culture house documents doctors",
            "hello world do fffffffffffffff"
            };
    auto real_result = ConverterJSON::GetTextDocuments();
    ASSERT_EQ(expected_result, real_result);
}

TEST(ConverterJSONTest, GetResponsesLimitTest) {
    int expected_value = 5;
    auto real_value = ConverterJSON::GetResponsesLimit();
    ASSERT_EQ(expected_value, real_value);
}

TEST(ConverterJSONTest, GetRequestsTest) {
    std::vector<std::string> expected_result = {
            "some words ww ee rr tt gg",
            "some",
            "words"
    };
    auto real_result = ConverterJSON::GetRequests();
    ASSERT_EQ(expected_result, real_result);
}

TEST(ConverterJSONTest, putAnswersTest) {
    std::vector<std::vector<std::pair<int, float>>> test_vec;
    test_vec = { { {1, 3.4}, {2, 0.3}}, {}, {{2, 8.5}}};
    ConverterJSON::putAnswers(test_vec);
    bool expected =
            std::filesystem::exists(FileHelper::findPath("JSONs/results.json"));
    ASSERT_TRUE(expected);
}


int test_main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
    return 0;
}