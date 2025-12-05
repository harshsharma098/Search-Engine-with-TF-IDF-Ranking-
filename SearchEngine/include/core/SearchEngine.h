#ifndef SEARCHENGINE_H
#define SEARCHENGINE_H

#include "DocumentIndexer.h"
#include "TFIDFCalculator.h"
#include <string>
#include <vector>
#include <map>

/**
 * @brief Represents a search result with document and relevance score.
 */
struct SearchResult {
    std::shared_ptr<Document> document;
    double score;

    // Default constructor (required for vector resize)
    SearchResult() : document(nullptr), score(0.0) {}

    // Parameterized constructor
    SearchResult(std::shared_ptr<Document> doc, double sc)
        : document(doc), score(sc) {}

    // Comparison operator (required for sorting)
    bool operator<(const SearchResult& other) const {
        return score > other.score; // Higher score = higher rank
    }
};



/**
 * @brief Main search engine class that orchestrates indexing and searching.
 * 
 * This class provides a high-level interface for indexing documents and
 * performing searches using TF-IDF ranking.
 */
class SearchEngine {
public:
    /**
     * @brief Constructor.
     */
    SearchEngine();

    /**
     * @brief Indexes a single document.
     * 
     * @param filePath Path to the document file
     * @return True if successful, false otherwise
     */
    bool indexDocument(const std::string& filePath);

    /**
     * @brief Indexes multiple documents.
     * 
     * @param filePaths Vector of file paths
     * @return Number of successfully indexed documents
     */
    int indexDocuments(const std::vector<std::string>& filePaths);

    /**
     * @brief Performs a search query and returns ranked results.
     * 
     * @param query The search query string
     * @param maxResults Maximum number of results to return (0 for all)
     * @return Vector of search results sorted by relevance score
     */
    std::vector<SearchResult> search(const std::string& query, size_t maxResults = 0) const;

    /**
     * @brief Gets the total number of indexed documents.
     * 
     * @return Number of documents
     */
    size_t getDocumentCount() const;

    /**
     * @brief Clears all indexed documents.
     */
    void clear();

    /**
     * @brief Gets all indexed document file paths.
     * 
     * @return Vector of file paths
     */
    std::vector<std::string> getIndexedFiles() const;

    /**
     * @brief Gets all indexed documents.
     * 
     * @return Vector of document pointers
     */
    const std::vector<std::shared_ptr<Document>>& getDocuments() const;

private:
    DocumentIndexer indexer;
    mutable std::unique_ptr<TFIDFCalculator> tfidfCalculator;
    
    /**
     * @brief Tokenizes and processes a query string.
     * 
     * @param query The query string
     * @return Vector of processed tokens
     */
    std::vector<std::string> processQuery(const std::string& query) const;
    
    /**
     * @brief Calculates relevance score for a document given query terms.
     * 
     * @param queryTerms Vector of query terms
     * @param document The document to score
     * @return Relevance score
     */
    double calculateRelevanceScore(const std::vector<std::string>& queryTerms, 
                                   const std::shared_ptr<Document>& document) const;
};

#endif // SEARCHENGINE_H

