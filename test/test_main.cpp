#include "gtest/gtest.h"

#include "custom_functions.cpp"
#include "converter_json.cpp"
#include "inverted_index.cpp"
#include "search_server.cpp"

TEST(ConverterJSONTest, EmptyDocumentsListTest) {
    ConverterJSON cj(TESTS_DIR);
    auto documents_path = cj.getTextDocuments();
    ASSERT_TRUE(!documents_path.empty());
}

TEST(ConverterJSONTest, GetTextDocumentsTest) {
    ConverterJSON cj(TESTS_DIR);
    std::vector<std::string> expected_result = {
            "../resources/file001.txt",
            "../resources/file002.txt",
            "../resources/file003.txt",
            "../resources/file004.txt"
            };
    auto real_result = cj.getTextDocuments();
    ASSERT_EQ(expected_result, real_result);
}

TEST(ConverterJSONTest, GetResponsesLimitTest) {
    int expected_value = 5;
    ConverterJSON cj(TESTS_DIR);
    auto real_value = cj.getResponsesLimit();
    ASSERT_EQ(expected_value, real_value);
}

TEST(ConverterJSONTest, GetRequestsTest) {
    std::vector<std::string> expected_result = {
            "some words ww ee rr tt gg",
            "some",
            "words"
    };
    ConverterJSON cj(TESTS_DIR);
    auto real_result = cj.getRequests();
    ASSERT_EQ(expected_result, real_result);
}

TEST(ConverterJSONTest, putAnswersTest) {
    std::vector<std::vector<RelativeIndex>> test_vec;
    test_vec = { { {1, 3.4}, {2, 0.3}}, {}, {{2, 8.5}}};
    ConverterJSON cj(TESTS_DIR);
    cj.putAnswers(test_vec);
    bool expected = std::filesystem::exists(ANSWERS_FILE_NAME);
    ASSERT_TRUE(expected);
}

void TestInvertedIndexFunctionality(
        const std::vector<std::string> &docs,
        const std::vector<std::string> &requests,
        const std::vector<Frequency> &expected
        ) {
    std::vector<Frequency> result;
    InvertedIndex idx;

    idx.updateDocumentBase(docs);

    for (auto &request : requests) {
        auto& word_count = idx.getWordCount(request);
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
    const std::vector<Frequency> expected = {
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
   const std::vector<Frequency> expected = {
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
                    {0, 0.7},
                    {1, 0.3}
            },
            {

            }
    };

    InvertedIndex idx;
    idx.updateDocumentBase(docs);

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
                    {0, 0.66666666666666663},
                    {1, 0.66666666666666663},
                    {2, 0.66666666666666663}
            }
    };

    InvertedIndex idx;
    idx.updateDocumentBase(docs);
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