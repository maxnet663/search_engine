#include "include/inverted_index.h"

#include <future>
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
    std::vector<std::thread> threads_pool;
    auto threads_limit = std::thread::hardware_concurrency();
    if (!threads_limit)
        threads_limit = 4; // def limit
    threads_pool.reserve(threads_limit);
    for (size_t i = 0; i < input_docs.size(); ++i) {
        threads_pool.emplace_back(&InvertedIndex::indexText
                , this
                , std::cref(input_docs[i])
                , i);
        if (threads_pool.size() >= threads_limit) {
            for (auto &thread : threads_pool) {
                thread.join();
            }
            threads_pool.clear();
        }
    }
}

const Frequency& InvertedIndex::getWordCount(const std::string &word) const {
    auto it = freq_dictionary.find(word);
    return it == freq_dictionary.end() ? nfound : it->second;
}

void InvertedIndex::indexText(const std::string &doc_path, size_t doc_id) {
    if (std::filesystem::exists(doc_path)) {
        FileReader reader(doc_path);
        if (!reader.is_open()) {
            std::lock_guard<std::mutex> horn(print_access);
            custom::print_yellow("Could not open the file " + doc_path);
        } else {
            std::string buf;
            while(reader >> buf) {
                std::lock_guard lock{dict_access};
                if (freq_dictionary.find(buf) == freq_dictionary.end()) {
                    freq_dictionary[buf].insert({doc_id, 1});
                } else {
                    freq_dictionary[buf][doc_id] += 1;
                }
            }
        }
    } else {
        std::lock_guard<std::mutex> horn(print_access);
        custom::print_yellow(doc_path + " does not exist");
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