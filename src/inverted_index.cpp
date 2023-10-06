#include "include/inverted_index.h"

#include <thread>

#include "include/custom_functions.h"
#include "include/formatting.h"

#define DEFAULT_THREADS_LIMIT 4

const freq_t InvertedIndex::nfound;

void
InvertedIndex::updateDocumentBase(const std::vector<std::string> &input_docs) {
    if (input_docs.empty()) {
        custom::print_yellow("DB: no documents to update");
        return;
    }
    freq_dictionary.clear();

    // reserves thread pool space
    std::vector<std::thread> threads_pool;
    auto threads_limit = std::thread::hardware_concurrency();
    if (!threads_limit)
        threads_limit = DEFAULT_THREADS_LIMIT;
    threads_pool.reserve(threads_limit);

    for (size_t i = 0; i < input_docs.size(); ++i) {
        if (!FileReader::isReadable(input_docs[i])) {
            custom::print_yellow("Could not read file "
                                 + input_docs[i]
                                 + ". Permission denied");
            continue;
        }
        if (!std::filesystem::exists(input_docs[i])) {
            custom::print_yellow("File "
                                 + input_docs[i]
                                 + " does not exists");
            continue;
        }
        FileReader reader(input_docs[i]);
        if (!reader.is_open()) {
            custom::print_yellow("Could not open file "
                                 + input_docs[i]
                                 + ". Something went wrong");
            continue;
        }

        // if file has opened launch threads
        while (threads_pool.size() <= threads_limit) {
            threads_pool.emplace_back(&InvertedIndex::indexText
                                      , this,
                                      std::ref(reader),
                                      static_cast<uint16_t>(i));
        }

        // we wait until they deal with him
        for (auto &thread : threads_pool) {
            thread.join();
        }
        threads_pool.clear();
    }
}

const freq_t& InvertedIndex::getWordCount(const std::string &word) const {
    auto it = freq_dictionary.find(word);
    return it == freq_dictionary.end() ? nfound : it->second;
}

void InvertedIndex::indexText(FileReader &reader, uint16_t doc_id) {
    std::string buf;
    while (reader >> buf) {
        std::lock_guard lock{dict_access};
        auto found = freq_dictionary.find(buf);
        if (found == freq_dictionary.end())
            freq_dictionary[buf].insert({doc_id, 1});
        else
            found->second[doc_id] += 1;
    }
}