#ifndef FORMATTING_H
#define FORMATTING_H

#include "unicode/unistr.h" // UnicodeString

namespace format {

    namespace unicode {

        /**
         * convert std::string from utf-8 to unicode
         * @param s string
         * @return UnicodeString
         */
        icu::UnicodeString makeUnicodeString(const std::string &s);

        /**
         * converts all characters to lowercase
         * @param s UnicodeString
         */
        void toLowerAll(icu::UnicodeString &s);

        /**
         * deletes whitespaces
         * @param s UnicodeString
         * @return UnicodeString
         */
        icu::UnicodeString deleteExtraSpaces(const icu::UnicodeString &s);

        /**
         * deletes punctuation marks
         * @param s UnicodeStrings
         * @return formatted Unicode string
         */
        icu::UnicodeString deletePunctuationMarks(const icu::UnicodeString &s);

        /**
         * converts Unicode string to utf-8 std::string
         * @param s UnicodeString
         * @return std::string
         */
        std::string makeUtfString(const icu::UnicodeString &s);

        /**
         * formatted UnicodeString: deletes whitespaces, punctuation
         * marks, converts all letters to lowercase
         * @param s UnicodeString
         */
        void convertToPlainText(icu::UnicodeString &s);
    }

    namespace utf {

        /**
         * delete punctuation marks, extra spaces and lowercase all letters
         * @param s string to format
         * @return reference to format string
         */
        void formatString(std::string &s);

        /**
         * lowercase all letters in s
         * @param s reference to string
         */
        void toLowerCase(std::string &s);

        /**
         * delete all punctuation marks in s
         * @param s reference to string
         */
        void deletePunctuationMarks(std::string &s);

        /**
         * delete all extra spaces
         * @param s reference to string
         */
        void deleteExtraSpaces(std::string &s);
    }
}

#endif //FORMATTING_H
