#include "search_server.h"

#include <algorithm>
#include <numeric>
#include <sstream>
#include <unordered_set>
#include <unordered_map>

std::vector<std::string> SearchServer::getUniqueWords(const std::string &text) {

    // set of unique words from requests
    std::unordered_set<std::string> uniqueRequests;
    std::stringstream data(text);
    std::string buf;
    while (data >> buf) {
        uniqueRequests.insert(buf);
    }

    return { uniqueRequests.begin(), uniqueRequests.end() };
}

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

inline size_t SearchServer::EntrySum(const std::vector<Entry>::iterator begin
                              , const std::vector<Entry>::iterator end) {
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

size_t SearchServer::getDocRelevance(const size_t &docId
                                           , const std::string &query) {

    auto queryFreq = _index.getWordCount(query);
    auto found = std::find_if(
            queryFreq.begin()
            , queryFreq.end()
            , [&](const Entry &entry){return entry.doc_id == docId;}
            );
    return found == queryFreq.end() ? 0 : found->count;
}

std::vector<std::vector<RelativeIndex>> SearchServer::search(
        const std::vector<std::string> &queries_input) {

    // result data
    std::vector<std::vector<RelativeIndex>> searchResults;

    // loop through each query
    for (const auto &query : queries_input) {

        // make a list of unique words
        std::vector<std::string> uniqueQueries = getUniqueWords(query);

        // sorts words in order of increasing frequency of occurrence
        sortQueries(uniqueQueries.begin(), uniqueQueries.end());

        // get list of relevant docs
        auto relevantDocs = getRelevantDocs(uniqueQueries);

        // if in the end there is not a single document left add empty list
        if (relevantDocs.empty()) {

            searchResults.emplace_back();

        } else {

            // get a list of documents and relevancy
            auto answers = getRelevantDocs(uniqueQueries);

            // sorts documents in descending order of relevance
            std::sort(answers.begin(), answers.end(), std::greater());

#ifdef TEST
            if (answers.size() > 5)
                answers.resize(5);
#endif

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
            searchResults.push_back(results);
        }
    }
    return searchResults;
}