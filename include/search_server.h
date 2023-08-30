#ifndef SEARCH_SERVER_H
#define SEARCH_SERVER_H

#include "include/inverted_index.h"

/**
 * Structure for forming the relevance of documents
 */
struct DocRelevance {
    size_t doc_id;
    size_t relevance;

    DocRelevance() : doc_id(0), relevance(0) {}

    explicit DocRelevance(const std::pair<size_t, size_t> &pair)
        : doc_id(pair.first), relevance(pair.second) {}

    bool operator>(const DocRelevance &right) const {
        return relevance == right.relevance ?
            doc_id < right.doc_id : relevance > right.relevance;
    }
};

struct RelativeIndex {
    size_t doc_id;
    float rank;

    bool operator==(const RelativeIndex &other) const {
        return (doc_id == other.doc_id && rank == other.rank);
    }
};

class SearchServer {

    const InvertedIndex &_index;

public:

    SearchServer() = delete;

    SearchServer(const SearchServer &other) = default;

    /**
     * @param idx pointer to the docs database
     */
    explicit SearchServer(const InvertedIndex &idx) : _index(idx) {};

    /**
     * Search query processing method
     * !async!
     * @param queries_input search queries taken from the file requests.json
     * @return returns a sorted list of relevant
     * responses for given requests
     */
    std::vector<std::vector<RelativeIndex>> search(
            const std::vector<std::string> &queries_input);

private:

    /**
     * generates a list of relevant documents and counts
     * relevance index for each document
     * @param unique_queries list of unique requests
     * @return list of relevant documents
     */
    std::vector<DocRelevance> getRelevantDocs(
            const std::vector<std::string> &unique_queries);

    /**
     * get a relevance index of doc relatively the query
     * @param doc_id
     * @param query
     * @return relevance of doc
     */
    size_t getDocRelevance(const size_t &doc_id, const std::string &query);

    /**
     * uses for async computing
     * method returns answer to the query
     * @param query string which represents a request
     * @return list with relative docs with their relevance
     */
    std::vector<RelativeIndex> makeRequest(const std::string &query);

};

#endif //SEARCH_SERVER_H