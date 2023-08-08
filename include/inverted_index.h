#ifndef INVERTED_INDEX_H
#define INVERTED_INDEX_H

#include "converter_json.h" // ConverterJSON

#include <vector> // vector
#include <map> // map

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

    // document content list
    std::vector<std::string> docs;

    // frequency dictionary
    std::map<std::string, std::vector<Entry>> freq_dictionary;

public:

    /**
     * default constructor
     */
    InvertedIndex() = default;

    /**
     * update or fill in the database of documents
     * on which we will then search
     * @param input_docs document's content
     */
    void UpdateDocumentBase(std::vector<std::string> input_docs);

    /**
     * method determines the number of occurrences
     * of a word in the loaded document base
     * @param word the word whose occurrence frequency is to be determined
     * @return a list with word frequency
     */
    std::vector<Entry> GetWordCount(const std::string &word);
};

#endif //INVERTED_INDEX_H
