#include "core/DocumentIndexer.h"
#include <sstream>
#include <filesystem>

DocumentIndexer::DocumentIndexer() 
    : tokenizer(std::make_unique<Tokenizer>()),
      stopWordRemover(std::make_unique<StopWordRemover>()) {
}

bool DocumentIndexer::indexDocument(const std::string& filePath) {
    std::string content = readFile(filePath);
    if (content.empty()) {
        return false;
    }
    
    // Check if document already indexed
    for (const auto& doc : documents) {
        if (doc->filePath == filePath) {
            return true; // Already indexed
        }
    }
    
    auto document = std::make_shared<Document>();
    document->filePath = filePath;
    
    // Extract filename from path
    std::filesystem::path path(filePath);
    document->fileName = path.filename().string();
    
    // Tokenize
    document->tokens = tokenizer->tokenize(content);
    
    // Remove stop words
    document->tokens = stopWordRemover->removeStopWords(document->tokens);
    
    // Build term frequency map
    document->termFrequency = buildTermFrequency(document->tokens);
    document->totalTerms = document->tokens.size();
    
    documents.push_back(document);
    return true;
}

int DocumentIndexer::indexDocuments(const std::vector<std::string>& filePaths) {
    int count = 0;
    for (const auto& path : filePaths) {
        if (indexDocument(path)) {
            count++;
        }
    }
    return count;
}

const std::vector<std::shared_ptr<Document>>& DocumentIndexer::getDocuments() const {
    return documents;
}

size_t DocumentIndexer::getDocumentCount() const {
    return documents.size();
}

void DocumentIndexer::clear() {
    documents.clear();
}

std::shared_ptr<Document> DocumentIndexer::getDocument(const std::string& filePath) const {
    for (const auto& doc : documents) {
        if (doc->filePath == filePath) {
            return doc;
        }
    }
    return nullptr;
}

std::string DocumentIndexer::readFile(const std::string& filePath) const {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        return "";
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

std::unordered_map<std::string, int> DocumentIndexer::buildTermFrequency(const std::vector<std::string>& tokens) const {
    std::unordered_map<std::string, int> tf;
    for (const auto& token : tokens) {
        tf[token]++;
    }
    return tf;
}

