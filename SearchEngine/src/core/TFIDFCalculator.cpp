#include "core/TFIDFCalculator.h"

TFIDFCalculator::TFIDFCalculator(const DocumentIndexer& indexer) 
    : indexer(indexer) {
}

double TFIDFCalculator::calculateTFIDF(const std::string& term, const std::shared_ptr<Document>& document) const {
    double tf = calculateTF(term, document);
    double idf = calculateIDF(term);
    return tf * idf;
}

double TFIDFCalculator::calculateTF(const std::string& term, const std::shared_ptr<Document>& document) const {
    if (document->totalTerms == 0) {
        return 0.0;
    }
    
    auto it = document->termFrequency.find(term);
    if (it == document->termFrequency.end()) {
        return 0.0;
    }
    
    // Normalized term frequency
    return static_cast<double>(it->second) / document->totalTerms;
}

double TFIDFCalculator::calculateIDF(const std::string& term) const {
    int df = getDocumentFrequency(term);
    if (df == 0) {
        return 0.0;
    }
    
    size_t totalDocs = indexer.getDocumentCount();
    if (totalDocs == 0) {
        return 0.0;
    }
    
    // IDF = log(total_documents / documents_containing_term)
    return std::log(static_cast<double>(totalDocs) / df);
}

int TFIDFCalculator::getDocumentFrequency(const std::string& term) const {
    // Check cache first
    auto it = documentFrequencyCache.find(term);
    if (it != documentFrequencyCache.end()) {
        return it->second;
    }
    
    // Calculate and cache
    int df = calculateDocumentFrequency(term);
    documentFrequencyCache[term] = df;
    return df;
}

int TFIDFCalculator::calculateDocumentFrequency(const std::string& term) const {
    int count = 0;
    const auto& documents = indexer.getDocuments();
    
    for (const auto& doc : documents) {
        if (doc->termFrequency.find(term) != doc->termFrequency.end()) {
            count++;
        }
    }
    
    return count;
}

