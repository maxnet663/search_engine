#include "inverted_index.h"
#include "converter_json.h" // ConverterJSON


void InvertedIndex::UpdateDocumentBase(std::vector<std::string> input_docs) {

}

std::vector<Entry> InvertedIndex::GetWordCount(const std::string &word) {

    // result data
    std::vector<Entry> result;

    // reviewing all documents
    for (size_t i = 0; i < docs.size(); ++i) {

        auto occurrences = countOccurrences(docs[i], word);

        // if there are not any occurrences of the word in the text
        // Entry is not created
        if (occurrences)
            result.push_back( {i, occurrences} );
    }

    return result;
}

size_t InvertedIndex::countOccurrences(const std::string &text, const std::string &word) {

    // the current index with which we will bypass the string
    size_t walk = 0;

    // counter of occurrences
    size_t occur_counter = 0;

    // while we can detect occurrence
    while (walk <= text.length() - word.length()) {

        // get the index of occurrence's start
        auto occur_begin = text.find(word, walk);

        // if there is not any occurrence return
        if (occur_begin == std::string::npos)
            return occur_counter;

        // get the index of occurrence's end
        auto occur_end = occur_begin + word.length();

        // if the occurrence is a single word, not part of a word
        if ( (occur_begin == 0 || text[occur_begin - 1] == ' ')
             && (text[occur_end] == ' ' || occur_end == text.length())) {

            occur_counter++;
        }

        // move the current index
        walk = occur_end;
    }

    return occur_counter;
}
