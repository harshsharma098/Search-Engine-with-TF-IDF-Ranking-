#ifndef STOPWORDREMOVER_H
#define STOPWORDREMOVER_H

#include <string>
#include <vector>
#include <unordered_set>

/**
 * @brief Removes common stop words from tokenized text.
 * 
 * Stop words are common words that don't carry significant meaning
 * for search purposes (e.g., "the", "a", "an", "in", etc.).
 */
class StopWordRemover {
public:
    /**
     * @brief Constructor that initializes the stop word list.
     */
    StopWordRemover();

    /**
     * @brief Removes stop words from a vector of tokens.
     * 
     * @param tokens Vector of tokens to filter
     * @return Vector of tokens with stop words removed
     */
    std::vector<std::string> removeStopWords(const std::vector<std::string>& tokens);

    /**
     * @brief Checks if a word is a stop word.
     * 
     * @param word The word to check
     * @return True if the word is a stop word, false otherwise
     */
    bool isStopWord(const std::string& word) const;

private:
    std::unordered_set<std::string> stopWords;
    
    /**
     * @brief Initializes the stop word set with common English stop words.
     */
    void initializeStopWords();
};

#endif // STOPWORDREMOVER_H

