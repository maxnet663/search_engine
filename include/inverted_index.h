#ifndef INVERTED_INDEX_H
#define INVERTED_INDEX_H

#include <vector>
#include <unordered_map>
#include <string>
#include <mutex>

#include "include/project_types.h"

/**
 * The class is a dictionary containing
 * unique words from indexed documents
 */
class InvertedIndex {

    DictionaryType freq_dictionary;
    std::mutex dict_access; // mutex to manage access to freq_dictionary
    std::mutex print_access;

public:

    /**
     * Default return value for queries that are not in the dictionary
     */
    static const Frequency nfound;

    /**
     * Constructs empty dictionary
     */
    InvertedIndex() = default;

    /**
     * Copy constructor without copying of mutex member
     * @param other another instance of InvertedIndex class
     */
    InvertedIndex(const InvertedIndex &other)
    : freq_dictionary(other.freq_dictionary) {};

    InvertedIndex& operator=(const InvertedIndex &right);

    InvertedIndex(InvertedIndex&& other) noexcept
    : freq_dictionary(std::move(other.freq_dictionary)) {};

    InvertedIndex& operator=(InvertedIndex&& right) noexcept;

    /**
     * Update or fill in the database of documents
     * on which we will then search
     * @param input_docs: paths to documents
     */
    void updateDocumentBase(const PathsList &input_docs);

    /**
     * Method determines the number of occurrences
     * of a word in the loaded document base
     * @param word: the word whose occurrence frequency is to be determined
     * @return const ref to a list with word frequency or nfound
     */
    const Frequency& getWordCount(const std::string &word) const;

private:

    /**
     * Extract texts from each docs in input_docs list
     * @param input_docs: list of paths to documents
     * @return list of texts in the same order
     */
    void indexText(const std::string &doc_path, size_t doc_id);

};

#endif //INVERTED_INDEX_H
