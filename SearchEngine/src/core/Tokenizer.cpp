#include "core/Tokenizer.h"
#include <sstream>
#include <regex>

std::vector<std::string> Tokenizer::tokenize(const std::string& text) {
    std::vector<std::string> tokens;
    std::string currentToken;
    
    for (char c : text) {
        if (std::isalnum(c) || c == '\'') {
            currentToken += std::tolower(c);
        } else {
            if (!currentToken.empty() && currentToken.length() > 1) {
                tokens.push_back(currentToken);
                currentToken.clear();
            } else {
                currentToken.clear();
            }
        }
    }
    
    // Add the last token if exists
    if (!currentToken.empty() && currentToken.length() > 1) {
        tokens.push_back(currentToken);
    }
    
    return tokens;
}

bool Tokenizer::isAlphanumeric(char c) {
    return std::isalnum(c) != 0;
}

std::string Tokenizer::toLower(const std::string& str) const {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

