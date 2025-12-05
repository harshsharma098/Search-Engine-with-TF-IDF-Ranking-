#ifndef TFIDFCALCULATOR_H
#define TFIDFCALCULATOR_H

#include "DocumentIndexer.h"
#include <unordered_map>
#include <cmath>

/**
 * @brief Calculates TF-IDF (Term Frequency-Inverse Document Frequency) scores.
 * 
 * TF-IDF is a numerical statistic that reflects how important a word is
 * to a document in a collection of documents. It increases proportionally
 * to the number of times a word appears in a document but is offset by
 * the frequency of the word in the corpus.
 */
class TFIDFCalculator {
public:
    /**
     * @brief Constructor.
     * 
     * @param indexer Reference to the document indexer
     */
    TFIDFCalculator(const DocumentIndexer& indexer);

    /**
     * @brief Calculates TF-IDF score for a term in a document.
     * 
     * @param term The term to calculate score for
     * @param document The document
     * @return TF-IDF score
     */
    double calculateTFIDF(const std::string& term, const std::shared_ptr<Document>& document) const;

    /**
     * @brief Calculates term frequency (TF) for a term in a document.
     * 
     * @param term The term
     * @param document The document
     * @return Term frequency (normalized)
     */
    double calculateTF(const std::string& term, const std::shared_ptr<Document>& document) const;

    /**
     * @brief Calculates inverse document frequency (IDF) for a term.
     * 
     * @param term The term
     * @return Inverse document frequency
     */
    double calculateIDF(const std::string& term) const;

    /**
     * @brief Gets the number of documents containing a term.
     * 
     * @param term The term to search for
     * @return Number of documents containing the term
     */
    int getDocumentFrequency(const std::string& term) const;

private:
    const DocumentIndexer& indexer;
    
    /**
     * @brief Caches document frequency for terms to avoid repeated calculations.
     */
    mutable std::unordered_map<std::string, int> documentFrequencyCache;
    
    /**
     * @brief Calculates and caches document frequency for a term.
     * 
     * @param term The term
     * @return Number of documents containing the term
     */
    int calculateDocumentFrequency(const std::string& term) const;
};

#endif // TFIDFCALCULATOR_H

