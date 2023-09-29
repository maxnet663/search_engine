#include "include/inverted_index.h"

#include <future>
#include <list>
#include <thread>
#include <iostream>

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

    // array of records: file id {word : occurrences }
    std::vector<std::unordered_map<std::string, size_t>> words_table;
    indexTexts(input_docs, words_table);
    std::cout << "docs loaded" << std::endl;

    std::vector<std::thread> threads_pool;
    auto threads_limit = std::thread::hardware_concurrency();
    if (!threads_limit)
        threads_limit = 4; // default threads limit
    threads_pool.reserve(threads_limit);
    for (size_t i = 0; i < words_table.size(); ++i) {
        threads_pool.emplace_back(&InvertedIndex::fillDictionary
                                 , this
                                 , std::cref(words_table[i])
                                 , i);
        if (threads_pool.size() >= threads_limit) {
            for (auto &thread : threads_pool)
                thread.join();
            threads_pool.clear();
        }
    }
}

const Frequency& InvertedIndex::getWordCount(const std::string &word) const {
    auto it = freq_dictionary.find(word);
    return it == freq_dictionary.end() ? nfound : it->second;
}

void InvertedIndex::fillDictionary(
        const std::unordered_map<std::string, size_t> &table
        , size_t doc_id) {
    for (auto &record : table) {
        std::lock_guard lock{dict_access};
            freq_dictionary[record.first].insert({doc_id, record.second});
    }
}

std::unordered_map<std::string, size_t>
InvertedIndex::loadText(const std::string &doc_path) {
    if (std::filesystem::exists(doc_path)) {
        FileReader reader(doc_path);
        if (!reader.is_open()) {
            std::lock_guard<std::mutex> lock(dict_access);
            custom::print_yellow("Could not open the file " + doc_path);
        } else {
            std::unordered_map<std::string, size_t> table;
            std::string buf;
            while(reader >> buf) {
                auto found = table.find(buf);
                if (found == table.end())
                    table[buf] = 1;
                else
                    table[buf] += 1;
            }
            return table;
        }
    } else {
        std::lock_guard<std::mutex> lock(dict_access);
        custom::print_yellow(doc_path + " does not exist");
    }
    return { };
}

void InvertedIndex::indexTexts(const PathsList &docs_paths
                             , std::vector<std::unordered_map<std::string, size_t>> dest) {
    std::vector<std::future<std::unordered_map<std::string, size_t>>> loader_pool;
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

void
InvertedIndex::fillWordsTable(std::vector<std::queue<std::string>> &texts
                              , std::vector<std::unordered_map<std::string, size_t>> &table) {
    std::vector<std::future<std::unordered_map<std::string, size_t>>> threads_pool;
    threads_pool.reserve(texts.size());
    for (auto & text : texts) {
        threads_pool.emplace_back(std::async(std::launch::async
                                             , &InvertedIndex::makeRecord
                                             , this
                                             , std::ref(text)));
    }

    if (!table.empty())
        table.clear();
    table.reserve(threads_pool.size());
    for (auto &record : threads_pool)
       table.emplace_back(record.get());
}

std::unordered_map<std::string, size_t>
InvertedIndex::makeRecord(std::queue<std::string> &text) {
    std::unordered_map<std::string, size_t> result;
    while(!text.empty()) {
        auto found = result.find(text.front());
        if (found == result.end())
            result[text.front()] = 1;
        else
            found->second += 1;
        text.pop();
    }
    return result;
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