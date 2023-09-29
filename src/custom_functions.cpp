#include "include/custom_functions.h"

#include <cstdint> // uint8_t
#include <list>
#include <filesystem>
#include <fstream>
#include <iostream>

#include "termcolor/termcolor.hpp"

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

std::string custom::getFileName(std::string s) {

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

    s.erase(s.begin(), walk + 1);

    return s;
}

size_t custom::countOccurrences(std::string_view text
                                , const std::string &word) {
    size_t counter = 0;
    while (!text.empty()) {
        text.remove_prefix(std::min(text.find_first_not_of(' '), text.length()));
        auto word_end = std::min(text.find_first_of(' '), text.length());
        counter += word == text.substr(0, word_end);
        text.remove_prefix(word_end);
    }
    return counter;
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

void custom::print_red(const std::string &msg) {
    std::cout << termcolor::red << msg << termcolor::reset << std::endl;
}

void custom::print_green(const std::string &msg) {
    std::cout << termcolor::green << msg << termcolor::reset << std::endl;
}

void custom::print_yellow(const std::string &msg) {
    std::cout << termcolor::yellow << msg << termcolor::reset << std::endl;
}

void custom::print_blue(const std::string &msg) {
    std::cout << termcolor::blue << msg << termcolor::reset << std::endl;
}
