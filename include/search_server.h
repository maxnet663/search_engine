#ifndef SEARCH_SERVER_H
#define SEARCH_SERVER_H

#include "inverted_index.h"

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

    InvertedIndex _index;

    /**
     * Sorts requests in order of increasing frequency of their occurrence
     * @param begin the beginning of a query consisting of unique words
     * @param end the end of a query consisting of unique words
     */
    inline void sortQueries(std::vector<std::string>::iterator begin
                     , std::vector<std::string>::iterator end);

    /**
     * helper function summing counts from all entries in range [begin, end)
     * @param begin start of the list to sum
     * @param end end of the list to sum
     * @return sum of all count from all entries in the range
     */
    inline size_t EntrySum(std::vector<Entry>::iterator begin
                           , std::vector<Entry>::iterator end);

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
     * @param docs list of documents
     * @param unique_queries list of queries
     * @return list with document IDs and their relevance
     */
    size_t getDocRelevance(const size_t &doc_id, const std::string &query);

public:

    /**
     * @param idx a reference to the class is passed to the class constructor
     * InvertedIndex, so that SearchServer can find out
     * the frequency of words found in request
     */
    explicit SearchServer(InvertedIndex &idx) : _index(idx) {};

    /**
     * Search query processing method
     * @param queries_input search queries taken from the file requests.json
     * @return returns a sorted list of relevant
     * responses for given requests
     */
    std::vector<std::vector<RelativeIndex>> search(
            const std::vector<std::string> &queries_input);

};

#endif //SEARCH_SERVER_H