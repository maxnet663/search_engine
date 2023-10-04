#include "include/custom_functions.h"

#include <list>
#include <filesystem>
#include <fstream>
#include <iostream>

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

std::vector<std::string> custom::getUniqueWords(const std::string &text) {
    std::list<std::string> unique_words;
    std::stringstream data(text);
    std::string buf;
    while (data >> buf) {
        unique_words.push_back(buf);
    }
    return { unique_words.begin(), unique_words.end() };
}

double custom::round(double num, int precision) {
    std::ostringstream os;
    os << std::fixed << std::setprecision(precision) << num;
    return std::stod(os.str());
}