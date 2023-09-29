#include "include/inverted_index.h"

#include <future>
#include <list>
#include <sstream>
#include <thread>

#include "include/custom_functions.h"
#include "include/formatting.h"
#include "include/file_reader.h"

const Frequency InvertedIndex::nfound;

void InvertedIndex::updateDocumentBase(const PathsList &input_docs) {
    if (input_docs.empty()) {
        custom::print_yellow("DB: no documents to update");
        return;
    }

    freq_dictionary.clear();

#ifdef TEST // for comfort testing
    docs_texts = input_docs;
#else
    loadFilesText(input_docs, docs_texts);
#endif

    // starts indexing
    std::vector<std::thread> threads_pool;
    auto threads_limit = std::thread::hardware_concurrency();
    if (!threads_limit)
        threads_limit = 4; // default limit
    threads_pool.reserve(threads_limit);
    for (const auto & docs_text : docs_texts) {
        // access to texts from docs is carried out by
        // constant reference, so there will be no race
        threads_pool.emplace_back(
                &InvertedIndex::parseInWords
                , this
                , docs_text.c_str());
        if (threads_pool.size() >= threads_limit) {
            for (auto &thread : threads_pool)
                thread.join();
            threads_pool.clear();
        }
    }

    for (auto &th : threads_pool) {
        th.join();
    }
}

const Frequency& InvertedIndex::getWordCount(const std::string &word) const {
    auto it = freq_dictionary.find(word);
    return it == freq_dictionary.end() ? nfound : it->second;
}

Frequency InvertedIndex::getWordFrequency(std::string_view &word) const {
    Frequency result;
    for (size_t i = 0; i < docs_texts.size(); ++i) {
        auto occurrences = custom::countOccurrences(docs_texts[i].c_str(), word);
        if (occurrences)
            result[i] = occurrences;
    }
    return result;
}

void InvertedIndex::parseInWords(std::string_view text) {
    while (!text.empty()) {
        auto begin = std::min(text.find_first_not_of(' '), text.length());
        auto end = std::min(text.find_first_of(' ', begin), text.length());
        auto word = text.substr(begin, end - begin);
        text.remove_prefix(end);
        if (word.empty())
            continue;
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
            target = std::move(word_count);
        }
    }
}

Text InvertedIndex::loadText(const std::string &doc_path) {
    if (std::filesystem::exists(doc_path)) {
        FileReader reader(doc_path);
        if (!reader.is_open()) {
            std::lock_guard<std::mutex> lock(dict_access);
            custom::print_yellow("Could not open the file " + doc_path);
        } else {
            return reader.getFormattedText();
        }
    } else {
        std::lock_guard<std::mutex> lock(dict_access);
        custom::print_yellow(doc_path + " does not exist");
    }
    return { };
}

void
InvertedIndex::loadFilesText(const PathsList &docs_paths, TextsList &dest) {
    std::vector<std::future<std::string>> loader_pool;
    loader_pool.reserve(docs_paths.size());
    for (auto &path : docs_paths) {
        loader_pool.emplace_back(std::async(std::launch::async
                                            , &InvertedIndex::loadText
                                            , this
                                            , path));
    }
    if (!dest.empty())
        dest.clear();
    dest.reserve(loader_pool.size());
    for (auto &text : loader_pool) {
        dest.emplace_back(text.get());
    }
}

InvertedIndex& InvertedIndex::operator=(InvertedIndex &&right) noexcept {
    freq_dictionary = std::move(right.freq_dictionary);
    return *this;
}

InvertedIndex& InvertedIndex::operator=(const InvertedIndex &right) {
    if (this != &right)
        freq_dictionary = right.freq_dictionary;
    return *this;
}

