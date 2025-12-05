#include "core/SearchEngine.h"
#include "core/Tokenizer.h"
#include "core/StopWordRemover.h"
#include <algorithm>

SearchEngine::SearchEngine() {
    tfidfCalculator = std::make_unique<TFIDFCalculator>(indexer);
}

bool SearchEngine::indexDocument(const std::string& filePath) {
    bool result = indexer.indexDocument(filePath);
    // Reset calculator to clear cache when new documents are added
    if (result) {
        tfidfCalculator = std::make_unique<TFIDFCalculator>(indexer);
    }
    return result;
}

int SearchEngine::indexDocuments(const std::vector<std::string>& filePaths) {
    int count = indexer.indexDocuments(filePaths);
    // Reset calculator to clear cache when new documents are added
    if (count > 0) {
        tfidfCalculator = std::make_unique<TFIDFCalculator>(indexer);
    }
    return count;
}

std::vector<SearchResult> SearchEngine::search(const std::string& query, size_t maxResults) const {
    std::vector<SearchResult> results;
    
    if (query.empty() || indexer.getDocumentCount() == 0) {
        return results;
    }
    
    // Process query
    std::vector<std::string> queryTerms = processQuery(query);
    if (queryTerms.empty()) {
        return results;
    }
    
    // Calculate scores for each document
    const auto& documents = indexer.getDocuments();
    for (const auto& doc : documents) {
        double score = calculateRelevanceScore(queryTerms, doc);
        if (score > 0.0) {
            results.push_back(SearchResult(doc, score));
        }
    }
    
    // Sort by score (descending)
    std::sort(results.begin(), results.end());
    
    // Limit results if specified
    if (maxResults > 0 && results.size() > maxResults) {
        results.resize(maxResults);
    }
    
    return results;
}

size_t SearchEngine::getDocumentCount() const {
    return indexer.getDocumentCount();
}

void SearchEngine::clear() {
    indexer.clear();
    tfidfCalculator = std::make_unique<TFIDFCalculator>(indexer);
}

std::vector<std::string> SearchEngine::getIndexedFiles() const {
    std::vector<std::string> files;
    const auto& documents = indexer.getDocuments();
    for (const auto& doc : documents) {
        files.push_back(doc->filePath);
    }
    return files;
}

const std::vector<std::shared_ptr<Document>>& SearchEngine::getDocuments() const {
    return indexer.getDocuments();
}

std::vector<std::string> SearchEngine::processQuery(const std::string& query) const {
    Tokenizer tokenizer;
    StopWordRemover stopWordRemover;
    
    std::vector<std::string> tokens = tokenizer.tokenize(query);
    return stopWordRemover.removeStopWords(tokens);
}

double SearchEngine::calculateRelevanceScore(const std::vector<std::string>& queryTerms, 
                                             const std::shared_ptr<Document>& document) const {
    double totalScore = 0.0;
    
    for (const auto& term : queryTerms) {
        double tfidf = tfidfCalculator->calculateTFIDF(term, document);
        totalScore += tfidf;
    }
    
    return totalScore;
}

