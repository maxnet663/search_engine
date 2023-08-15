#include "search_server.h"
#include <unordered_set>
#include <algorithm>
#include <numeric>
#include <sstream>

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
            auto answers =
                    getDocsRelevance(relevantDocs, uniqueQueries);

            // sorts documents in descending order of relevance
            sortRelevantDocs(answers);

            if (answers.size() > 5)
                answers.resize(5);

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

void SearchServer::sortRelevantDocs(std::vector<DocRelevance> &answers) {
    std::sort(answers.begin()
              , answers.end()
              , std::greater());
}

size_t SearchServer::EntrySum(const std::vector<Entry>::iterator begin
                              , const std::vector<Entry>::iterator end) {
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
// todo choose another container for faster search in it
    std::vector<size_t> result;
    for (const auto &query : unique_queries) {
        auto queryFreq = _index.getWordCount(query);
        for (const auto &entry : queryFreq) {
            auto found = std::find_if(result.begin()
                    , result.end()
                    , [&](const size_t &docId){return entry.doc_id == docId;});
            if ( found == result.end())
                result.push_back(entry.doc_id);
        }
    }
    return result;
}

std::vector<Entry> SearchServer::getCommonDocs(
        const std::vector<Entry> &first, const std::vector<Entry> &second) {

    //result data
    std::vector<Entry> result;

    for (auto &doc : first) {
        auto found = std::find_if(
                second.begin()
                , second.end()
                , [&](const Entry &entry){return doc.doc_id == entry.doc_id;});

        // if two lists have common document
        // add the document to result
        if (found != second.end()) {
            result.push_back(doc);
        }
    }
    return result;
}

std::vector<DocRelevance> SearchServer::getDocsRelevance(
        const std::vector<size_t> &docs
        , const std::vector<std::string> &queries) {

    //result data
    std::vector<DocRelevance> docsRelevance;

    // get relevance for each doc from docs
    for (const auto &doc : docs) {

        size_t relevance = 0;

        for (const auto &query: queries) {

            auto queryDocs = _index.getWordCount(query);

            for (const auto &entry : queryDocs) {
                //if query occurs in doc increase relevance
                if (entry.doc_id == doc) {
                    relevance += entry.count;
                }

            }
        }
        docsRelevance.push_back( {doc, relevance} );
    }
    return docsRelevance;
}
