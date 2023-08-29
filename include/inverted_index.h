#ifndef INVERTED_INDEX_H
#define INVERTED_INDEX_H

#include <vector>
#include <unordered_map>
#include <string>
#include <mutex>

// a record with the document number and
// the number of occurrences of a certain word in it
struct Entry {
    size_t doc_id;
    size_t count;

    // the operator is required for tests
    bool operator==(const Entry &other) const {
        return (doc_id == other.doc_id && count == other.count);
    }
};

class InvertedIndex {

    std::vector<std::string> docs_texts;
    std::unordered_map<std::string, std::vector<Entry>> freq_dictionary;
    std::mutex dict_access; // mutex to manage access to freq_dictionary

public:

    InvertedIndex() = default;

    /**
     * needed because the mutex class has had its copy
     * constructor removed, so we can't use the default copy constructor
     * @param other another instance of InvertedIndex class
     */
    InvertedIndex(const InvertedIndex &other)
        : docs_texts(other.docs_texts)
        , freq_dictionary(other.freq_dictionary) {};

    InvertedIndex(InvertedIndex&& other) noexcept
        : docs_texts(std::move(other.docs_texts))
        , freq_dictionary(std::move(other.freq_dictionary)) {};

    /**
     * update or fill in the database of documents
     * on which we will then search
     * @param input_docs document's content
     */
    void updateDocumentBase(const std::vector<std::string> &input_docs);

    /**
     * method determines the number of occurrences
     * of a word in the loaded document base
     * @param word the word whose occurrence frequency is to be determined
     * @return a list with word frequency
     */
    std::vector<Entry> getWordCount(const std::string &word);

private:

    /**
     * fill freq_dictionary by the unique words from the text
     * @param text file's text to get unique words
     */
    void addUniqueWords(const std::string &text);

    /**
     * searches for occurrences of a word in array docs of document texts
     * and prepares a frequency for the word
     * @param word
     * @return frequency of the word
     */
    std::vector<Entry> getWordFrequency(const std::string &word);

    /**
     * extract texts from each docs in input_docs list
     * @param input_docs list of paths to documents
     * @return list of texts in the same order
     */
    std::vector<std::string> getFilesTexts(
            const std::vector<std::string> &input_docs);

};

#endif //INVERTED_INDEX_H
