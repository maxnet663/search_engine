#include "inverted_index.h"
#include "converter_json.h" // ConverterJSON


void InvertedIndex::UpdateDocumentBase(std::vector<std::string> input_docs) {

}

std::vector<Entry> InvertedIndex::GetWordCount(const std::string &word) {
    std::vector<Entry> result(docs.size());
    for (size_t i = 0; i < docs.size(); ++i) {
        result[i] = {i, countOccurrences(docs[i], word)};
    }
    return result;
}

size_t InvertedIndex::countOccurrences(const std::string &text, const std::string &word) {
    size_t walk = 0;
    size_t occur_counter = 0;
    while (walk <= text.length() - word.length()) {

        auto occur_begin = text.find(word, walk);
        if (occur_begin == std::string::npos)
            return occur_counter;
        auto occur_end = occur_begin + word.length();

        if ( (occur_begin == 0 || text[occur_begin - 1] == ' ')
             && (text[occur_end] == ' ' || occur_end == text.length())) {
            occur_counter++;
        }
        walk = occur_end;
    }
    return occur_counter;
}
