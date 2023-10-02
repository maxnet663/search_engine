#include "include/search_server.h"

#include <sstream>
#include <future>
#include <list>
#include <queue>

#include "include/custom_functions.h"

std::vector<answer_t>
SearchServer::search(const std::vector<std::string> &queries_input) {

    std::list<std::vector<RelativeIndex>> search_results;

    std::queue<std::future<std::vector<RelativeIndex>>> requests_pool;
    for (const auto &query : queries_input) {
        requests_pool.push(std::async(std::launch::async
                                      , &SearchServer::makeRequest
                                      , this
                                      , query));
    }
    while (!requests_pool.empty()) {
        search_results.push_back(requests_pool.front().get());
        requests_pool.pop();
    }
    return { search_results.begin(), search_results.end() };
}

std::vector<DocRelevance>
SearchServer::getRelevantDocs(const std::vector<std::string> &unique_queries) {

    // unordered_map<doc_id, Relevance>
    std::unordered_map<size_t, size_t> result;

    for (const auto &query : unique_queries) {
        // get a list of documents that contain the query
        auto &query_freq = _index.getWordCount(query);
        for (const auto &entry : query_freq) {
            // enter the document and its relevance
            // relatively the query into the table
            if (result.find(entry.first) == result.end()) {
                result[entry.first] = entry.second;
            } else {
                result[entry.first] += entry.second;
            }
        }
    }
    // at the output we get a final table with documents and their relevance
    return { result.begin(), result.end() };
}

answer_t SearchServer::makeRequest(const std::string &query) {
    auto unique_queries = custom::getUniqueWords(query);
    auto answers = getRelevantDocs(unique_queries);

    // if in the end there is not a single document left - add empty list
    if (answers.empty()) {
        return { };
    } else {
        // sorts documents in descending order of relevance
        std::sort(answers.begin(), answers.end(), std::greater());

        auto maxRelevance = answers.begin()->relevance;
        std::vector<RelativeIndex> results(answers.size());

        for (size_t i = 0; i < results.size(); ++i) {
            results[i] = {
                    answers[i].doc_id,
                    (double) answers[i].relevance / (double) maxRelevance
            };
        }
        return results;
    }
}