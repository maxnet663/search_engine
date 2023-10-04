#ifndef INVERTED_INDEX_H
#define INVERTED_INDEX_H

#include <vector>
#include <unordered_map>
#include <string>
#include <mutex>

/**
 *  The representation of words frequency
 *  std::unordered_map<doc_id, occurrences>
 */
typedef std::unordered_map<uint16_t, uint32_t> freq_t;

/**
 * data structure for frequency dictionary
 */
typedef std::unordered_map<std::string, freq_t> dict_t;

/**
 * The class is a dictionary which containing
 * unique words from indexed documents
 */
class InvertedIndex {

    dict_t freq_dictionary;
    std::mutex dict_access; // mutex to manage access to freq_dictionary
    std::mutex print_access; // mutex to access to standard output

public:

    /**
     * Default return value for queries that are not in the dictionary
     */
    static const freq_t nfound;

    /**
     * Update or fill in the database of documents
     * on which we will then search
     * @param input_docs: paths to documents
     */
    void updateDocumentBase(const std::vector<std::string> &input_docs);

    /**
     * Method determines the number of occurrences
     * of a word in the loaded document base
     * @param word: the word whose occurrence frequency is to be determined
     * @return const ref to a list with word frequency or nfound
     */
    const freq_t& getWordCount(const std::string &word) const;

private:

    /**
     * Extract texts from each docs in input_docs list
     * @param input_docs: list of paths to documents
     * @return list of texts in the same order
     */
    void indexText(const std::string &doc_path, uint16_t doc_id);
};

#endif //INVERTED_INDEX_H
