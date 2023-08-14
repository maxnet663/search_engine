#ifndef SEARCH_SERVER_H
#define SEARCH_SERVER_H

#include "inverted_index.h"

/**
 * Structure for forming the relevance of documents
 */
struct DocRelevance {
    size_t doc_id;
    size_t relevance;
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
     * the function generates a list of unique words from the text
     * @param text string of one or more words
     * @return list of unique words
     */
    std::vector<std::string> getUniqueWords(const std::string &text);

    /**
     * Sorts requests in order of increasing frequency of their occurrence
     * @param begin the beginning of a query consisting of unique words
     * @param end the end of a query consisting of unique words
     */
    void sortQueries(std::vector<std::string>::iterator begin
                     , std::vector<std::string>::iterator end);

    /**
     * sorts the list of documents in descending order of relevance
     * @param answers List of documents and their relevances
     */
    void sortRelevantDocs(std::vector<DocRelevance> &answers);

    /**
     * By the first, rarest word from the list, finds all documents in which
     * word occurs. Next, it looks for matches between the next word and
     * this list of documents. So by each next word.
     * @param unique_queries list of unique requests
     * @return a list of documents in which all words from unique_queries occur
     */
    std::vector<size_t> getRelevantDocs(
            const std::vector<std::string> &unique_queries);

    /**
     * function is used as a helper in getRelevantDocs
     * searches for common documents in two lists
     * @param first list of docs
     * @param second list of docs
     * @return list of common docs
     */
    std::vector<Entry> getCommonDocs(const std::vector<Entry> &first
                                     , const std::vector<Entry> &second);

    /**
     * Gets a list with document IDs and their relevance
     * @param docs list of documents
     * @param queries list of queries
     * @return list with document IDs and their relevance
     */
    std::vector<DocRelevance> getDocsRelevance(
            const std::vector<size_t> &docs
            , const std::vector<std::string> &queries);

    /**
     * helper function summing counts from all entries in range [begin, end)
     * @param begin start of the list to sum
     * @param end end of the list to sum
     * @return summ of all count from all entries in the range
     */
    size_t EntrySum(const std::vector<Entry>::iterator begin
                    , const std::vector<Entry>::iterator end);

public:

    /**
     *
     * @param idx a reference to the class is passed to the class constructor
     * InvertedIndex, so that SearchServer can find out
     * the frequency of words found in request
     */
    SearchServer(InvertedIndex &idx) : _index(idx) {};

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