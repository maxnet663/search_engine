#include "file_helper.h"
#include "custom_limits.h"

#include <filesystem> //path directory_iterator current_path
#include <fstream>  // ifstream ofstream
#include <regex> //regex regex_match

std::string FileHelper::getFileText(const std::string &filename) {

    // open stream for reading
    std::ifstream ifs(filename);

    //check words number in file
    if (std::distance(std::istream_iterator<std::string>(ifs)
            , std::istream_iterator<std::string>()) > MAX_WORDS_NUMBER) {
        throw std::length_error(std::string("number of words in file ")
                                + filename  + std::string(" greater than ")
                                + std::to_string(MAX_WORDS_NUMBER));
    }

    // back to the start of file
    ifs.clear();
    ifs.seekg(0);

    std::string buf; // buffer for word processing
    std::string text;//storage for text

    // reserve memory
    text.reserve(std::filesystem::file_size(filename));

    // read one word at a time
    while (ifs >> buf) {

        //check word's length
        if (buf.length() > MAX_WORD_LENGTH) {
            throw std::length_error(std::string("one of words from file:")
                                    + filename
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

void FileHelper::formatString(std::string &s)  {

    if (s.length() == 0) {
        return;
    }
    //delete punctuation marks
    deletePunctuationMarks(s);

    //lowercase letters
    toLowerCase(s);

    //delete extra spaces
    deleteExtraSpaces(s);
}

int FileHelper::wordsCounter(const std::string &s) {

    // flag
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

void FileHelper::toLowerCase(std::string &s) {

    for (auto &ch : s) {
        if (isupper(ch)) {
            ch = tolower(ch);
        }
    }

}

void FileHelper::deletePunctuationMarks(std::string &s) {
    s.erase(std::remove_if(s.begin()
                    , s.end()
                    , [](char &ch){
                        return std::ispunct(ch); })
            , s.end());
}

void FileHelper::deleteExtraSpaces(std::string &s) {

    // use two pointers
    auto back = s.begin();
    auto front = s.begin();

    // search first non space ch
    while (*front == ' ') {
        ++front;
    };

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

void FileHelper::writeJsonToFile(const nlohmann::json &jsonText, const std::string &path) {

    // if file already exists we should remove it
    if (std::filesystem::exists(path)) {
        remove(path.data());
    }

    // open stream for writing
    std::ofstream dest(path, std::ios::out);
    dest << jsonText;
    dest.close();

}

std::string FileHelper::findPath(std::string name) {

    //format name
    // pointers to the beginning of the string
    auto walk = name.begin();

    // skip unnecessary symbols such as '../../'
    while(!isalpha(*walk)) {
        walk++;
    }

    // remove them
    name.erase(name.begin(), walk);

    // if last ch is slashed
    if (name.back() == '/' || name.back() == '\\') {
        name.pop_back();
    }

    // returns empty string if name is incorrect
    if (name.empty()) {
        return {};
    }

    // PROJECT_ROOT_PATH is a macros contains path to
    //project root directory
    std::filesystem::path rootPath = PROJECT_ROOT_PATH;

    // result
    std::filesystem::path found;

    // get iterator to search in root directory
    std::filesystem::recursive_directory_iterator content(rootPath);
    for (const auto &curPath: content) {
        if (std::regex_match(curPath.path().string(), std::regex(".*" + name)))
            found = curPath.path();
    }

    // return empty string if func could not find the path
    // founded path is a directory adds slash to the end
    return is_directory(found) ? found.string() + "/" :  found.string();
}
