#include "search_server.h"
#include <unordered_set>
#include <algorithm>
#include <numeric>
#include <sstream>

std::vector<std::vector<RelativeIndex>> SearchServer::search(
        const std::vector<std::string> &queries_input) {

    std::vector<std::vector<RelativeIndex>> searchResults;

    for (const auto &query : queries_input) {
        std::vector<std::string> uniqueQueries = getUniqueWords(query);
        sortQueries(uniqueQueries.begin(), uniqueQueries.end());
        auto relevantDocs = getRelevantDocs(uniqueQueries);
        if (relevantDocs.empty()) {
            searchResults.emplace_back();
        } else {
            auto docsRelevance =
                    getDocsRelevance(relevantDocs, uniqueQueries);
            auto maxRelevance = std::max_element(
                    docsRelevance.begin()
                    , docsRelevance.end()
            );
            std::vector<RelativeIndex> result(relevantDocs.size());
            for (size_t i = 0; i < relevantDocs.size(); ++i) {
                result[i] = {
                        relevantDocs[i]
                        , (float) docsRelevance[i] / (float) *maxRelevance
                };
            }
            searchResults.push_back(result);
        }
    }
    return searchResults;
}

std::vector<std::string> SearchServer::getUniqueWords(const std::string &text) {
    std::unordered_set<std::string> uniqueRequests;
    std::stringstream data(text);
    std::string buf;
    while (data >> buf) {
        uniqueRequests.insert(buf);
    }

    return { uniqueRequests.begin(), uniqueRequests.end() };
}

void SearchServer::sortQueries(std::vector<std::string>::iterator begin
        , std::vector<std::string>::iterator end) {
    auto addition = [&](const std::string &x, const std::string &y) {
        auto x_freq = _index.getWordCount(x);
        auto y_freq = _index.getWordCount(y);
        auto x_sum = EntrySum(x_freq.begin(), x_freq.end());
        auto y_sum = EntrySum(y_freq.begin(), y_freq.end());
        return x_sum < y_sum;
    };
    std::sort(begin, end, addition);
}

size_t SearchServer::EntrySum(std::vector<Entry>::iterator begin
        , std::vector<Entry>::iterator end) {
    return std::accumulate(
            begin
            , end
            , 0
            , [](size_t num, const Entry &entry) {
                return num + entry.count;
            });
}

std::vector<size_t> SearchServer::getRelevantDocs(
        const std::vector<std::string> &unique_queries) {

    // we form a list of relevant documents according to the data
    // from the freq_dict for the rarest query
    auto relevantDocs = _index.getWordCount(*unique_queries.begin());

    // start round the queries from second query
    auto currentQuery = unique_queries.begin() + 1;
    while(currentQuery != unique_queries.end() && !relevantDocs.empty()) {

        // form list of relevant docs for current query
        auto queryDocs = _index.getWordCount(*currentQuery);
        relevantDocs = getCommonDocs(relevantDocs, queryDocs);
        currentQuery++;
    }

    std::vector<size_t> results(relevantDocs.size());
    for (size_t i = 0; i < relevantDocs.size(); ++i) {
        results[i] = relevantDocs[i].doc_id;
    }
    return results;
}

std::vector<Entry> SearchServer::getCommonDocs(
        const std::vector<Entry> &first, const std::vector<Entry> &second) {
    std::vector<Entry> result;
    for (auto &doc : first) {
        auto found = std::find_if(
                second.begin()
                , second.end()
                , [&](const Entry &entry){return doc.doc_id == entry.doc_id;});
        if (found != second.end()) {
            result.push_back(doc);
        }
    }
    return result;
}

std::vector<size_t> SearchServer::getDocsRelevance(
        const std::vector<size_t> &docs
        , const std::vector<std::string> &queries) {

    std::vector<size_t> docsRelevance;
    for (const auto &doc : docs) {
        size_t relevance = 0;
        for (const auto &query: queries) {
            auto queryDocs = _index.getWordCount(query);
            for (const auto &entry : queryDocs) {
                if (entry.doc_id == doc) {
                    relevance += entry.count;
                }
            }
        }
        docsRelevance.push_back(relevance);
    }
    return docsRelevance;
}
