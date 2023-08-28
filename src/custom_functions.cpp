#include "include/custom_functions.h"
#include "include/project_constants.h"

#include <filesystem>
#include <fstream>
#include <unordered_set>

std::string custom::getFileText(const std::string &file_name) {

    // open stream for reading
    std::ifstream ifs(file_name);

    if (!ifs.is_open()) {
        throw std::filesystem::filesystem_error("Could not open the file"
                , file_name
                , std::make_error_code(std::errc::no_such_file_or_directory));
    }

    //check words number in file
    if (std::distance(std::istream_iterator<std::string>(ifs)
            , std::istream_iterator<std::string>()) > MAX_WORDS_NUMBER) {

        throw std::length_error(std::string("number of words in file ")
                                + file_name + std::string(" greater than ")
                                + std::to_string(MAX_WORDS_NUMBER));
    }

    // back to the start of file
    ifs.clear();
    ifs.seekg(0);

    std::string buf; // buffer for word processing
    std::string text;//storage for text

    // reserve memory
    text.reserve(std::filesystem::file_size(file_name));

    // read one word at a time
    while (ifs >> buf) {

        //check word's length
        if (buf.length() > MAX_WORD_LENGTH) {
            throw std::length_error(std::string("one of words from file:")
                                    + file_name
                                    + std::string( " has length greater than ")
                                    + std::to_string(MAX_WORD_LENGTH));
        }

        //put the word to the storage
        text += buf + " ";
    }

    // close stream
    ifs.close();

    // format string
    formatString(text);

    return text;
}

void custom::formatString(std::string &s)  {
    if (s.length() == 0) {
        return;
    }

    deletePunctuationMarks(s);

    toLowerCase(s);

    deleteExtraSpaces(s);
}

size_t custom::wordsCounter(const std::string &s) {
    bool is_word = false;
    int words_counter = 0;

    // go through all the string
    for (const auto &ch: s) {

        // if letters and numbers run out and we met space
        if (!isalnum(ch) && is_word) {
            words_counter++;
            is_word = false;
            continue;
        }

        // if its letter or digit
        if (isalnum(ch)) {
            is_word = true;
            continue;
        }
    }

    // if the string ends with a word
    return words_counter + is_word;
}

void custom::toLowerCase(std::string &s) {

    std::for_each(s.begin(), s.end(), [](char &ch){ ch = tolower(ch); });

}

void custom::deletePunctuationMarks(std::string &s) {
    s.erase(std::remove_if(s.begin()
                    , s.end()
                    , [](char &ch){return std::ispunct(ch);})
            , s.end());
}

void custom::deleteExtraSpaces(std::string &s) {

    // use two pointers
    auto back = s.begin();
    auto front = s.begin();

    // search first non space ch
    while (*front == ' ') {
        ++front;
    }

    while (*front) {

        // if met one space and prev ch not space
        // write the ch
        if (*front != ' ' || *(front - 1) != ' ') {
            *back++ = *front;
        }

        // move on the string
        front++;
    }

    // delete extra ch
    s.resize(back - s.begin());

    // if last ch is space delete it
    if (*(s.end() - 1) == ' ') {
        s.pop_back();
    }
}

void custom::writeJsonToFile(const nlohmann::json &json_text
                             , const std::string &path) {

    // if file already exists we should remove it
    if (std::filesystem::exists(path)) {
        remove(path.data());
    }

    // open stream for writing
    std::ofstream dest(path, std::ios::out);
    dest << std::setw(2) << json_text;
    dest.close();

}

std::string custom::getFileName(std::string s) {

    //format name
    // pointers to the end of the string
    auto walk = s.end() - 1;

    // if its directory
    if (*walk == '/' || *walk == '\\') {
        walk--;
    }

    // search for the first slash
    while(*walk != '/' && *walk != '\\' && walk != s.begin()) {
        walk--;
    }

    // remove them
    s.erase(s.begin(), walk + 1);

    return s;
}

size_t custom::countOccurrences(const std::string &text, const std::string &word) {

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
             && (text[occur_end] == ' ' || occur_end == text.length()) ) {

            occur_counter++;
        }

        // move the current index
        walk = occur_end;
    }

    return occur_counter;
}

std::vector<std::string> custom::getUniqueWords(const std::string &text) {

    // set of unique words from requests
    std::unordered_set<std::string> uniqueRequests;
    std::stringstream data(text);
    std::string buf;
    while (data >> buf) {
        uniqueRequests.insert(buf);
    }

    return { uniqueRequests.begin(), uniqueRequests.end() };
}
