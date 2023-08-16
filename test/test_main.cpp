#include "custom_functions.cpp"
#include "converter_json.cpp"
#include "inverted_index.cpp"
#include "search_server.cpp"
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
};

TEST(TestCaseSearchServer, TestSimple) {
    const std::vector<std::string> docs = {
            "milk milk milk milk water water water",
            "milk water water",
            "milk milk milk milk milk water water water water water",
            "americano cappuccino"
    };

    const std::vector<std::string> request = {"milk water", "sugar"};
    const std::vector<std::vector<RelativeIndex>> expected = {
            {
                    {2, 1},
                    {0, 0.7f},
                    {1, 0.3f}
            },
            {

            }
    };

    InvertedIndex idx;
    idx.UpdateDocumentBase(docs);

    SearchServer srv(idx);

    std::vector<std::vector<RelativeIndex>> result = srv.search(request);

    ASSERT_EQ(result, expected);
}

TEST(TestCaseSearchServer, TestTop5) {
    const std::vector<std::string> docs = {
            "london is the capital of great britain",
            "paris is the capital of france",
            "berlin is the capital of germany",
            "rome is the capital of italy",
            "madrid is the capital of spain",
            "lisboa is the capital of portugal",
            "bern is the capital of switzerland",
            "moscow is the capital of russia",
            "kiev is the capital of ukraine",
            "minsk is the capital of belarus",
            "astana is the capital of kazakhstan",
            "beijing is the capital of china",
            "tokyo is the capital of japan",
            "bangkok is the capital of thailand",
            "welcome to moscow the capital of russia the third rome",
            "amsterdam is the capital of netherlands",
            "helsinki is the capital of finland",
            "oslo is the capital of norway",
            "stockholm is the capital of sweden",
            "riga is the capital of latvia",
            "tallinn is the capital of estonia",
            "warsaw is the capital of poland",
    };
    const std::vector<std::string> request = {"moscow is the capital of russia"};
    const std::vector<std::vector<RelativeIndex>> expected = {
            {
                    {7, 1},
                    {14, 1},
                    {0, 0.666666687},
                    {1, 0.666666687},
                    {2, 0.666666687}
            }
    };
    InvertedIndex idx;
    idx.UpdateDocumentBase(docs);
    SearchServer srv(idx);
    std::vector<std::vector<RelativeIndex>> result = srv.search(request);
    if (result[0].size() > 5) {
        result[0].resize(5);
    }
    ASSERT_EQ(result, expected);
}

int test_main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
    return 0;
}