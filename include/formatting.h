#ifndef FORMATTING_H
#define FORMATTING_H

#include "unicode/unistr.h" // UnicodeString

/**
 * namespace defines two other namespaces
 * for formatting strings in the unicode and utf-8
 * encodings accordingly
 */
namespace format {

    /**
     * defines functions for working with Unicode strings
     */
    namespace unicode {

        /**
         * Convert std::string from utf-8 to unicode
         * @param s: string
         * @return UnicodeString
         */
        icu::UnicodeString makeUnicodeString(const std::string &s);

        /**
         * Converts all characters to lowercase
         * @param s: UnicodeString
         */
        void toLowerAll(icu::UnicodeString &s);

        /**
         * Deletes whitespaces
         * @param s: UnicodeString
         * @return UnicodeString
         */
        icu::UnicodeString deleteExtraSpaces(const icu::UnicodeString &s);

        /**
         * Deletes punctuation marks
         * @param s: UnicodeStrings
         * @return formatted Unicode string
         */
        icu::UnicodeString deletePunctuationMarks(const icu::UnicodeString &s);

        /**
         * Converts Unicode string to utf-8 std::string
         * @param s: UnicodeString
         * @return std::string
         */
        std::string makeUtfString(const icu::UnicodeString &s);

        /**
         * Formatted UnicodeString: deletes whitespaces, punctuation
         * marks, converts all letters to lowercase
         * @param s: UnicodeString
         */
        void convertToPlainText(icu::UnicodeString &s);
    }

    /**
     * Defines functions for working with utf-8 strings
     */
    namespace utf {

        /**
         * Delete punctuation marks, extra spaces and lowercase all letters
         * @param s: string to format
         * @return reference to format string
         */
        void formatString(std::string &s);

        /**
         * Lowercase all letters in s
         * @param s: reference to string
         */
        void toLowerCase(std::string &s);

        /**
         * Delete all punctuation marks in s
         * @param s: reference to string
         */
        void deletePunctuationMarks(std::string &s);

        /**
         * Delete all extra spaces
         * @param s: reference to string
         */
        void deleteExtraSpaces(std::string &s);
    }
}

#endif //FORMATTING_H
