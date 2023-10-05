#include "gtest/gtest.h"

#include "custom_functions.cpp"
#include "converter_json.cpp"
#include "inverted_index.cpp"
#include "search_server.cpp"

TEST(ConverterJSONTest, EmptyDocumentsListTest) {
    ConverterJSON cj(TESTS_DIR);
    auto documents_path = cj.getDocumentsPaths();
    ASSERT_TRUE(!documents_path.empty());
}

TEST(ConverterJSONTest, GetTextDocumentsTest) {
    ConverterJSON cj(TESTS_DIR);
    std::vector<std::string> expected_result = {
            "../resources_test/file001.txt",
            "../resources_test/file002.txt",
            "../resources_test/file003.txt",
            "../resources_test/file004.txt"
            };
    auto real_result = cj.getDocumentsPaths();
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
        const std::vector<freq_t> &expected
        ) {
    std::vector<freq_t> result;
    InvertedIndex idx;

    idx.updateDocumentBase(docs);

    for (auto &request : requests) {
        auto& word_count = idx.getWordCount(request);
        result.push_back((word_count));
    }

    ASSERT_EQ(result, expected);
}

TEST(TestCaseInvertedIndex, TestBasic) {

    std::string path = TESTS_DIR;
    const std::vector<std::string> docs = {
            path + "/" + "resources_test/test_basic1.txt",
            path + "/" + "resources_test/test_basic2.txt"
    };
    const std::vector<std::string> requests = {"london", "the"};
    const std::vector<freq_t> expected = {
            { {0, 1} },
            { {0, 1}, {1, 3} }
    };
    TestInvertedIndexFunctionality(docs, requests, expected);
};

TEST(TestCaseInvertedIndex, TestInvertedIndexMissingWord) {
    std::string path = TESTS_DIR;
   const std::vector<std::string> docs = {
           path + "/" + "resources_test/test_missing_word1.txt",
           path + "/" + "resources_test/test_missing_word2.txt"
   };
   const std::vector<std::string> requests = {"m", "statement"};
   const std::vector<freq_t> expected = {
           { },
           { {1, 1} }
   };
   TestInvertedIndexFunctionality(docs, requests, expected);
};

TEST(TestCaseSearchServer, TestSimple) {
    std::string path = TESTS_DIR;
    const std::vector<std::string> docs = {
            path + "/" + "resources_test/test_simple1.txt",
            path + "/" + "resources_test/test_simple2.txt",
            path + "/" + "resources_test/test_simple3.txt",
            path + "/" + "resources_test/test_simple4.txt"
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
    std::string path = TESTS_DIR;
    const std::vector<std::string> docs = {
            path + "/" + "resources_test/test1.txt",
            path + "/" + "resources_test/test2.txt",
            path + "/" + "resources_test/test3.txt",
            path + "/" + "resources_test/test4.txt",
            path + "/" + "resources_test/test5.txt",
            path + "/" + "resources_test/test6.txt",
            path + "/" + "resources_test/test7.txt",
            path + "/" + "resources_test/test8.txt",
            path + "/" + "resources_test/test9.txt",
            path + "/" + "resources_test/test10.txt",
            path + "/" + "resources_test/test11.txt",
            path + "/" + "resources_test/test12.txt",
            path + "/" + "resources_test/test13.txt",
            path + "/" + "resources_test/test14.txt",
            path + "/" + "resources_test/test15.txt",
            path + "/" + "resources_test/test16.txt",
            path + "/" + "resources_test/test17.txt",
            path + "/" + "resources_test/test18.txt",
            path + "/" + "resources_test/test19.txt",
            path + "/" + "resources_test/test20.txt",
            path + "/" + "resources_test/test21.txt",
            path + "/" + "resources_test/test22.txt",
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