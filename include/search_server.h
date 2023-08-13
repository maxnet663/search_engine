#ifndef SEARCH_SERVER_H
#define SEARCH_SERVER_H

#include "inverted_index.h"

struct RelativeIndex {
    size_t doc_id;
    float rank;

    bool operator==(const RelativeIndex &other) const {
        return (doc_id == other.doc_id && rank == other.rank);
    }
};

class SearchServer {

    InvertedIndex _index;

    std::vector<std::string> getUniqueWords(const std::string &text);

    void sortQueries(std::vector<std::string>::iterator begin
                     , std::vector<std::string>::iterator end);

    std::vector<size_t> getRelevantDocs(
            const std::vector<std::string> &unique_queries);

    std::vector<Entry> getCommonDocs(const std::vector<Entry> &first
                                     , const std::vector<Entry> &second);

    std::vector<size_t> getDocsRelevance(
            const std::vector<size_t> &docs
            , const std::vector<std::string> &queries);

    size_t EntrySum(std::vector<Entry>::iterator begin
                    , std::vector<Entry>::iterator end);

public:

    SearchServer(InvertedIndex &idx) : _index(idx) {};

    std::vector<std::vector<RelativeIndex>> search(
            const std::vector<std::string> &queries_input);

};

#endif //SEARCH_SERVER_H