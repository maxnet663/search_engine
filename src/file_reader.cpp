#include "include/file_reader.h"

void FileReader::open(const std::string &file_path) {
    if (read_stream.is_open())
        read_stream.close();
    path = file_path;
    read_stream.open(file_path);
}

std::string FileReader::getText() {
    if (read_stream.eof()) {
        read_stream.clear();
        read_stream.seekg(0, std::ios_base::beg);
    }
    std::string text;
    text.reserve(std::filesystem::file_size(path));
    std::string buf;
    auto separator = " ";
    while (read_stream >> buf) {
        text.append(buf);
        if (!read_stream.eof())
            text.append(separator);
    }
    return text;
}

std::string FileReader::getFormattedText() {
    auto uc_text = format::unicode::makeUnicodeString(getText());
    format::unicode::convertToPlainText(uc_text);
    return format::unicode::makeUtfString(uc_text);
}

bool FileReader::isReadable(const std::string &file_name) {
    std::filesystem::path path(file_name);
    if (!exists(path))
        return false;

    auto file_perms = std::filesystem::status(path).permissions();
    auto read_perms = std::filesystem::perms::owner_read;
    return std::filesystem::perms::none != (file_perms & read_perms);
}

bool FileReader::isWriteable(const std::string &file_name) {
    std::filesystem::path path(file_name);
    auto file_perms = std::filesystem::status(path).permissions();
    auto write_perms = std::filesystem::perms::owner_write;
    return std::filesystem::perms::none != (file_perms & write_perms);
}

bool FileReader::operator>>(std::string &dest) {
    if (read_stream.eof()) return false;
    std::string word;
    read_stream >> word;
    auto uc_word = format::unicode::makeUnicodeString(word);
    format::unicode::convertToPlainText(uc_word);
    dest.clear();
    dest.append(format::unicode::makeUtfString(uc_word));
    return true;
}
