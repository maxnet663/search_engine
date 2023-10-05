#include "include/inverted_index.h"

#include <thread>

#include "include/custom_functions.h"
#include "include/formatting.h"
#include "include/file_reader.h"

#define DEFAULT_THREADS_LIMIT 4

const freq_t InvertedIndex::nfound;

void
InvertedIndex::updateDocumentBase(const std::vector<std::string> &input_docs) {
    if (input_docs.empty()) {
        custom::print_yellow("DB: no documents to update");
        return;
    }
    std::vector<std::thread> threads_pool;
    auto threads_limit = std::thread::hardware_concurrency();
    if (!threads_limit)
        threads_limit = DEFAULT_THREADS_LIMIT;
    threads_pool.reserve(threads_limit);
    for (size_t i = 0; i < input_docs.size(); ++i) {
        threads_pool.emplace_back(&InvertedIndex::indexText
                                  , this
                                  , std::cref(input_docs[i])
                                  , static_cast<uint16_t>(i));
        if (threads_pool.size() >= threads_limit
        || i == input_docs.size() - 1) {
            for (auto &thread : threads_pool) {
                thread.join();
            }
            threads_pool.clear();
        }
    }
}

const freq_t& InvertedIndex::getWordCount(const std::string &word) const {
    auto it = freq_dictionary.find(word);
    return it == freq_dictionary.end() ? nfound : it->second;
}

void InvertedIndex::indexText(const std::string &doc_path, uint16_t doc_id) {
    if (std::filesystem::exists(doc_path)) {
        FileReader reader(doc_path);
        if (!reader.is_open()) {
            std::lock_guard<std::mutex> horn{print_access};
            custom::print_yellow("Could not open the file " + doc_path);
        } else {
            std::string buf;
            while(reader >> buf) {
                std::lock_guard lock{dict_access};
                auto found = freq_dictionary.find(buf);
                if (found == freq_dictionary.end()) {
                    freq_dictionary[buf].insert({doc_id, 1});
                } else {
                    found->second[doc_id] += 1;
                }
            }
        }
    } else {
        std::lock_guard<std::mutex> horn{print_access};
        custom::print_yellow(doc_path + " does not exist");
    }
}