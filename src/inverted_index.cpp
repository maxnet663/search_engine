#include "inverted_index.h"

#include "custom_functions.h"

#include <thread>
#include <sstream>

std::vector<Entry> InvertedIndex::getWordFrequency(const std::string &word) {

    // result data
    std::vector<Entry> result;

    // reviewing all documents
    for (size_t i = 0; i < docs.size(); ++i) {

        auto occurrences = custom::countOccurrences(docs[i], word);

        // if there are not any occurrences of the word in the text
        // Entry is not created
        if (occurrences)
            result.push_back( {i, occurrences} );
    }

    return result;
}

void InvertedIndex::addUniqueWords(const std::string &text) {

    // we will extract the words one by one
    std::stringstream data(text);
    std::string word;

    while (data >> word) {

        // we protect our container from the race
        // RAII concept will not let us forget to unlock the mutex
        std::unique_lock<std::mutex> lock(dict_acces);

        if (freq_dictionary.find(word) == freq_dictionary.end()) {

            //we add the word to the dictionary so that following threads
            // can see that we are preparing to add entries for this word
            auto &target = freq_dictionary[word];

            // while records are being formed, we open access
            // to the container so as not to delay following threads
            lock.unlock();

            // the function accepts a word by constant reference,
            // and also accesses the docs array read-only, so it is safe
            auto word_count = getWordFrequency(word);

            // when adding a new word, we lock the dictionary again
            // and prevent the race
            lock.lock();

            target = std::move(word_count);
        }
    }
}

void InvertedIndex::UpdateDocumentBase(std::vector<std::string> input_docs) {

    // clear old data if it necessary
    if (!docs.empty()) {
        docs.clear();
    }
    if (!freq_dictionary.empty()) {
        freq_dictionary.clear();
    }

    // we no longer need input_docs
    docs = std::move(input_docs);

    // starts threads
    std::vector<std::thread> threads(docs.size());
    for (size_t i = 0; i < docs.size(); ++i) {

        // access to lines from docs is carried out by
        // constant reference, so there will be no race
        threads[i] = std::thread(
                &InvertedIndex::addUniqueWords
                , this
                , std::ref(docs[i])
                );
    }

    // waits for threads
    for (auto &th : threads) {
        th.join();
    }
}

std::vector<Entry> InvertedIndex::getWordCount(const std::string &word) {
    return
        freq_dictionary.empty() ? std::vector<Entry>() : freq_dictionary[word];
}
