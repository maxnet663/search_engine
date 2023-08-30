#include "include/search_server.h"

#include <sstream>
#include <future>
#include <list>
#include <queue>

#include "include/custom_functions.h"

std::vector<DocRelevance> SearchServer::getRelevantDocs(
        const std::vector<std::string> &unique_queries) {

    // unordered_map<doc_id, Relevance>
    std::unordered_map<size_t, size_t> result;

    for (const auto &query : unique_queries) {

        auto queryFreq = _index.getWordCount(query);

        for (const auto &entry : queryFreq) {
            if (result.find(entry.doc_id) == result.end()) {
                result[entry.doc_id] = getDocRelevance(entry.doc_id, query);
            } else {
                result[entry.doc_id] += getDocRelevance(entry.doc_id, query);
            }
        }
    }
    return { result.begin(), result.end() };
}

size_t SearchServer::getDocRelevance(const size_t &doc_id
                                           , const std::string &query) {
    auto query_freq = _index.getWordCount(query);
    auto found = std::find_if(
            query_freq.begin()
            , query_freq.end()
            , [&](const Entry &entry){return entry.doc_id == doc_id;}
            );
    return found == query_freq.end() ? 0 : found->count;
}

std::vector<RelativeIndex> SearchServer::makeRequest(const std::string &query) {
    std::vector<std::string> unique_queries = custom::getUniqueWords(query);
    auto answers = getRelevantDocs(unique_queries);

    // if in the end there is not a single document left add empty list
    if (answers.empty()) {

        return {};

    } else {

        // sorts documents in descending order of relevance
        std::sort(answers.begin(), answers.end(), std::greater());

        auto maxRelevance = answers.begin()->relevance;
        std::vector<RelativeIndex> results(answers.size());

        for (size_t i = 0; i < results.size(); ++i) {
            results[i] = {
                    answers[i].doc_id,
                    (float) answers[i].relevance / (float) maxRelevance
            };
        }
        return results;
    }
}

std::vector<std::vector<RelativeIndex>> SearchServer::search(
        const std::vector<std::string> &queries_input) {

    std::list<std::vector<RelativeIndex>> search_results;

    std::queue<std::future<std::vector<RelativeIndex>>> threads_pool;
    for (const auto &query : queries_input) {
        threads_pool.push(std::async(&SearchServer::makeRequest, this, query));
    }
    while (!threads_pool.empty()) {
        search_results.push_back(threads_pool.front().get());
        threads_pool.pop();
    }
    return { search_results.begin(), search_results.end() };
}
