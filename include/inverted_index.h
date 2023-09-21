#ifndef INVERTED_INDEX_H
#define INVERTED_INDEX_H

#include <vector>
#include <unordered_map>
#include <string>
#include <mutex>

struct Entry;


typedef
std::unordered_map<std::string, std::unordered_map<size_t, size_t>> DictionaryType;
typedef std::string Text;
typedef std::vector<std::string> TextsList;
typedef std::vector<std::string> PathsList;
typedef std::unordered_map<size_t, size_t> Frequency;

class InvertedIndex {

    TextsList docs_texts;
    DictionaryType freq_dictionary;
    std::mutex dict_access; // mutex to manage access to freq_dictionary

public:

    /**
     * default return value for queries that are not in the dictionary
     */
    static const Frequency nfound;

    InvertedIndex() = default;

    /**
     * needed because the mutex class has had its copy
     * constructor removed, so we can't use the default copy constructor
     * @param other another instance of InvertedIndex class
     */
    InvertedIndex(const InvertedIndex &other)
    : docs_texts(other.docs_texts), freq_dictionary(other.freq_dictionary) {};

    InvertedIndex& operator=(const InvertedIndex &right);

    InvertedIndex(InvertedIndex&& other) noexcept
    : docs_texts(std::move(other.docs_texts))
    , freq_dictionary(std::move(other.freq_dictionary)) {};

    InvertedIndex& operator=(InvertedIndex&& right) noexcept;

    /**
     * update or fill in the database of documents
     * on which we will then search
     * @param input_docs paths to documents
     */
    void updateDocumentBase(const PathsList &input_docs);

    /**
     * method determines the number of occurrences
     * of a word in the loaded document base
     * @param word the word whose occurrence frequency is to be determined
     * @return const ref to a list with word frequency or nfound
     */
    const Frequency& getWordCount(const std::string &word) const;

private:

    /**
     * fill freq_dictionary by the unique words from the text
     * !uses for threads!
     * @param text file's text to get unique words
     */
    void parseInWords(const Text &text);

    /**
     * searches for occurrences of a word in array docs of document texts
     * and prepares a frequency for the word
     * @param word
     * @return frequency of the word
     */
    Frequency getWordFrequency(const std::string &word) const;

    /**
     * extract texts from each docs in input_docs list
     * @param input_docs list of paths to documents
     * @return list of texts in the same order
     */
    Text loadText(const std::string &doc_path);

    /**
     * loads texts from file's list to dest
     * !dest will be cleared!
     * @param docs_paths list of paths to files
     * @param dest destination to load
     */
    void loadFilesText(const PathsList &docs_paths, TextsList &dest);

};

#endif //INVERTED_INDEX_H
