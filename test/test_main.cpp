#include "file_helper.cpp"
#include "converter_json.cpp"
#include "inverted_index.cpp"
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
            "hello world do ffffffffffffffffff"
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
            std::filesystem::exists(JSONS_DIR RESULTS_FILE_NAME);
    ASSERT_TRUE(expected);
}

void TestInvertedIndexFunctionality(
        const std::vector<std::string> &docs,
        const std::vector<std::string> &requests,
        const std::vector<std::vector<Entry>> &expected
        ) {
    std::vector<std::vector<Entry>> result;
    InvertedIndex idx;

    idx.UpdateDocumentBase(docs);

    for (auto &request : requests) {
        std::vector<Entry> word_count = idx.getWordCount(request);
        result.push_back((word_count));
    }

    ASSERT_EQ(result, expected);
}

TEST(TestCaseInvertedIndex, TestBasic) {
    const std::vector<std::string> docs = {
            "london is the capital of great britain",
            "big ben is the nickname for the great bell of the striking clock"
    };
    const std::vector<std::string> requests = {"london", "the"};
    const std::vector<std::vector<Entry>> expected = {
            { {0, 1} },
            { {0, 1}, {1, 3} }
    };
    TestInvertedIndexFunctionality(docs, requests, expected);
};

TEST(TestCaseInvertedIndex, TestInvertedIndexMissingWord) {
   const std::vector<std::string> docs = {
           "a b c d e f g h i j k l",
           "statement"
   };
   const std::vector<std::string> requests = {"m", "statement"};
   const std::vector<std::vector<Entry>> expected = {
           { },
           { {1, 1} }
   };
   TestInvertedIndexFunctionality(docs, requests, expected);
}

int test_main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
    return 0;
}