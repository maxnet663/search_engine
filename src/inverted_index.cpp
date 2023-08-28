#include "include/inverted_index.h"

#include "include/custom_functions.h"

#include <list>
#include <sstream>
#include <iostream>
#include <thread>

void InvertedIndex::UpdateDocumentBase(const std::vector<std::string> &input_docs) {
    if (!docs.empty()) {
        docs.clear();
    }
    if (!freq_dictionary.empty()) {
        freq_dictionary.clear();
    }

#ifdef TEST // for comfort testing
    docs = std::move(input_docs);
#else
    docs = getFilesTexts(input_docs);
#endif

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

std::vector<Entry> InvertedIndex::getWordFrequency(const std::string &word) {
    std::vector<Entry> result;

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
            // and also accesses the docs array read-only, so it is safe
            auto word_count = getWordFrequency(word);

            // when adding a new word, we lock the dictionary again
            // and prevent the race
            lock.lock();

            target = std::move(word_count);
        }
    }
}

std::vector<std::string> InvertedIndex::getFilesTexts(
        const std::vector<std::string> &input_docs) {
    //it is faster, in case when we have a lot if docs
    std::list<std::string> texts;
    for (const auto &docs_path : input_docs) {
        if (std::filesystem::exists(docs_path)) {
            try {
                texts.push_back(custom::getFileText(docs_path));
            }
            catch (std::length_error &ex) {
                std::cout << ex.what() << std::endl;
            }
        } else {
            std::cerr << docs_path << " does not exist\n";
        }
    }
    return { texts.begin(), texts.end() };
}
