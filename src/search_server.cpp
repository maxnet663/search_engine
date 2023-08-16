#include "search_server.h"

#include "custom_functions.h"

#include <algorithm>
#include <numeric>
#include <sstream>

inline void SearchServer::sortQueries(std::vector<std::string>::iterator begin
        , std::vector<std::string>::iterator end) {

    auto countSumComp = [&](const std::string &x, const std::string &y) {
        auto xFreq = _index.getWordCount(x);
        auto yFreq = _index.getWordCount(y);
        auto xCountSum = EntrySum(xFreq.begin(), xFreq.end());
        auto yCountSum = EntrySum(yFreq.begin(), yFreq.end());
        return xCountSum < yCountSum;
    };

    std::sort(begin, end, countSumComp);
}

inline size_t SearchServer::EntrySum(std::vector<Entry>::iterator begin
                                     , std::vector<Entry>::iterator end) {
    return std::accumulate(
            begin
            , end
            , 0
            , [](const size_t &num, const Entry &entry)
            { return num + entry.count; }
            );
}

std::vector<DocRelevance> SearchServer::getRelevantDocs(
        const std::vector<std::string> &unique_queries) {

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

std::vector<std::vector<RelativeIndex>> SearchServer::search(
        const std::vector<std::string> &queries_input) {

    // result data
    std::vector<std::vector<RelativeIndex>> search_results;

    // loop through each query
    for (const auto &query : queries_input) {

        // make a list of unique words
        std::vector<std::string> unique_queries = custom::getUniqueWords(query);

        // sorts words in order of increasing frequency of occurrence
        sortQueries(unique_queries.begin(), unique_queries.end());

        // get list of relevant docs
        auto relevant_docs = getRelevantDocs(unique_queries);

        // if in the end there is not a single document left add empty list
        if (relevant_docs.empty()) {

            search_results.emplace_back();

        } else {

            // get a list of documents and relevancy
            auto answers = getRelevantDocs(unique_queries);

            // sorts documents in descending order of relevance
            std::sort(answers.begin(), answers.end(), std::greater());

            // get maxRelevance
            auto maxRelevance = answers[0].relevance;

            // form result
            std::vector<RelativeIndex> results(answers.size());

            for (size_t i = 0; i < results.size(); ++i) {
                results[i] = {
                        answers[i].doc_id,
                        (float) answers[i].relevance / (float) maxRelevance
                };
            }
            search_results.push_back(results);
        }
    }
    return search_results;
}