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
    buf.reserve(BUFFER_SIZE);
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
