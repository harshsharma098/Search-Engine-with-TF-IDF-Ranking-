#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <string>
#include <vector>
#include <algorithm>
#include <cctype>

/**
 * @brief Tokenizes text into words by splitting on whitespace and punctuation.
 * 
 * This class provides functionality to break down text into individual tokens
 * (words) by removing punctuation and converting to lowercase.
 */
class Tokenizer {
public:
    /**
     * @brief Tokenizes a given text string.
     * 
     * Splits the text into words, removes punctuation, and converts to lowercase.
     * 
     * @param text The input text to tokenize
     * @return Vector of tokenized words
     */
    std::vector<std::string> tokenize(const std::string& text);

    /**
     * @brief Checks if a character is alphanumeric.
     * 
     * @param c The character to check
     * @return True if alphanumeric, false otherwise
     */
    static bool isAlphanumeric(char c);

private:
    /**
     * @brief Converts a string to lowercase.
     * 
     * @param str The string to convert
     * @return Lowercase version of the string
     */
    std::string toLower(const std::string& str) const;
};

#endif // TOKENIZER_H

