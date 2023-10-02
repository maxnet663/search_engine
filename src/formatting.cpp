#include "include/formatting.h"

#include <algorithm> // for_each remove_if

#include "unicode/regex.h" // RegexMatcher
#include "unicode/errorcode.h" // UErrorCode

icu::UnicodeString
format::unicode::makeUnicodeString(const std::string &s) {
    return icu::UnicodeString(icu::UnicodeString::fromUTF8(s));
}

void format::unicode::toLowerAll(icu::UnicodeString &s) {
    s.toLower();
}

icu::UnicodeString
format::unicode::deleteExtraSpaces(const icu::UnicodeString &s) {
    UErrorCode errors = U_ZERO_ERROR;
    icu::RegexMatcher ext_spaces("\\s+", s, 0, errors);
    return ext_spaces.replaceAll(u" ", errors);
}

icu::UnicodeString
format::unicode::deletePunctuationMarks(const icu::UnicodeString &s) {
    UErrorCode errors = U_ZERO_ERROR;
    icu::RegexMatcher punct_marks("[[:punct:]]", s, 0, errors);
    return punct_marks.replaceAll(u"", errors);
}

std::string format::unicode::makeUtfString(const icu::UnicodeString &s) {
    std::string res;
    return s.toUTF8String(res);
}

void format::unicode::convertToPlainText(icu::UnicodeString &s) {
    s.toLower();
    s = deletePunctuationMarks(s);
    s = deleteExtraSpaces(s);
    s.trim();
}

void format::utf::formatString(std::string &s)  {
    if (s.length() == 0) {
        return;
    }

    deletePunctuationMarks(s);

    toLowerCase(s);

    deleteExtraSpaces(s);
}

void format::utf::toLowerCase(std::string &s) {
    std::for_each(s.begin()
            , s.end()
            , [](char &ch){ ch = tolower(ch); });
}

void format::utf::deletePunctuationMarks(std::string &s) {
    s.erase(std::remove_if(s.begin()
                    , s.end()
                    , [](char &ch){return std::ispunct(ch);})
            , s.end());
}

void format::utf::deleteExtraSpaces(std::string &s) {

    // use two pointers
    auto back = s.begin();
    auto front = s.begin();

    // search first non space ch
    while (std::isspace(*front)) {
        ++front;
    }

    while (*front) {

        if (*front == '\t')
            *front = ' ';

        // if met one space and prev ch not space
        // write the ch
        if (*front != ' ' || *(front - 1) != ' ') {
            *back++ = std::move(*front);
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