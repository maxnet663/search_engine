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

size_t custom::countOccurrences(const std::string &text
                                , const std::string &word) {
    size_t walk = 0;
    size_t occur_counter = 0;

    // while we can detect occurrence
    while (walk <= text.length() - word.length()) {

        // get the index of occurrence's start
        auto occur_begin = text.find(word, walk);

        // if there is not any occurrence
        if (occur_begin == std::string::npos)
            return occur_counter;

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

bool custom::isReadable(const std::string &file_name) {
    std::filesystem::path path(file_name);
    if (!exists(path))
        return false;

    auto file_perms = std::filesystem::status(path).permissions();
    auto read_perms = std::filesystem::perms::owner_read;
    return std::filesystem::perms::none != (file_perms & read_perms);
}

bool custom::isWriteable(const std::string &file_name) {
    std::filesystem::path path(file_name);
    if (!exists(path))
        return false;

    auto file_perms = std::filesystem::status(path).permissions();
    auto write_perms = std::filesystem::perms::owner_write;
    return std::filesystem::perms::none != (file_perms & write_perms);
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
