#include "include/inverted_index.h"

#include <list>
#include <sstream>
#include <iostream>
#include <thread>

#include "include/custom_functions.h"

const std::vector<Entry> InvertedIndex::nfound;

void InvertedIndex::updateDocumentBase(const std::vector<std::string> &input_docs) {
    if (input_docs.empty()) {
        std::cerr << "DB: no documents to update" << std::endl;
        return;
    }
    if (!docs_texts.empty()) {
        docs_texts.clear();
    }
    if (!freq_dictionary.empty()) {
        freq_dictionary.clear();
    }

#ifdef TEST // for comfort testing
    docs_texts = std::move(input_docs);
#else
    docs_texts = getFilesTexts(input_docs);
#endif

    // starts threads
    std::vector<std::thread> threads_pool(docs_texts.size());
    for (size_t i = 0; i < docs_texts.size(); ++i) {

        // access to lines from docs is carried out by
        // constant reference, so there will be no race
        threads_pool[i] = std::thread(
                &InvertedIndex::addUniqueWords
                , this
                , std::ref(docs_texts[i]));
    }
    for (auto &th : threads_pool) {
        th.join();
    }
}

const std::vector<Entry>& InvertedIndex::getWordCount(const std::string &word) const {
    auto it = freq_dictionary.find(word);
    return it == freq_dictionary.end() ? nfound : it->second;
}

std::vector<Entry> InvertedIndex::getWordCount(const std::string &word) {
    auto it = freq_dictionary.find(word);
    return it == freq_dictionary.end() ? std::vector<Entry>() : it->second;
}

std::vector<Entry>
InvertedIndex::getWordFrequency(const std::string &word) const {
    std::vector<Entry> result;

    for (size_t i = 0; i < docs_texts.size(); ++i) {
        auto occurrences = custom::countOccurrences(docs_texts[i], word);
        if (occurrences)
            result.push_back( {i, occurrences} );
    }
    return result;
}

void InvertedIndex::addUniqueWords(const std::string &text) {
    std::stringstream data(text);
    std::string word;

    while (data >> word) {

        // we protect our container from the race
        // RAII concept will not let us forget to unlock the mutex
        std::unique_lock<std::mutex> lock(dict_access);

        if (freq_dictionary.find(word) == freq_dictionary.end()) {

            //we add the word to the dictionary so that following threads
            // can see that we are preparing to add entries for this word
            auto &target = freq_dictionary[word];

            // while records are being formed, we open access
            // to the container so as not to delay following threads
            lock.unlock();

            // the function accepts a word by constant reference,
            // and also accesses the docs_texts array read-only, so it is safe
            auto word_count = getWordFrequency(word);

            // when adding a new word, we lock the dictionary again
            // and prevent the race
            lock.lock();

            target = std::move(word_count);
        }
    }
}

std::vector<std::string> InvertedIndex::getFilesTexts(
        const std::vector<std::string> &input_docs) const {

    //it is faster, in case when we have a lot if docs
    std::list<std::string> texts;
    for (const auto &docs_path : input_docs) {
        if (std::filesystem::exists(docs_path)) {
            try {
                texts.push_back(custom::getFileText(docs_path));
            }
            catch (std::length_error &ex) {
                std::cerr << ex.what() << std::endl;
            }
        } else {
            std::cerr << docs_path << " does not exist\n";
        }
    }
    return { texts.begin(), texts.end() };
}

InvertedIndex& InvertedIndex::operator=(InvertedIndex &&right) noexcept {
    docs_texts = std::move(right.docs_texts);
    freq_dictionary = std::move(right.freq_dictionary);
    return *this;
}

InvertedIndex& InvertedIndex::operator=(const InvertedIndex &right) {
    if (this != &right) {
        docs_texts = right.docs_texts;
        freq_dictionary = right.freq_dictionary;
    }
    return *this;
}


