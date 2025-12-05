#ifndef DOCUMENTINDEXER_H
#define DOCUMENTINDEXER_H

#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <fstream>
#include <memory>
#include "Tokenizer.h"
#include "StopWordRemover.h"

/**
 * @brief Represents a document with its content and metadata.
 */
struct Document {
    std::string filePath;
    std::string fileName;
    std::vector<std::string> tokens;
    std::unordered_map<std::string, int> termFrequency; // term -> count
    int totalTerms;
    
    Document() : totalTerms(0) {}
};

/**
 * @brief Indexes documents by tokenizing and counting term frequencies.
 * 
 * This class is responsible for reading text files, tokenizing their content,
 * removing stop words, and building an index of term frequencies per document.
 */
class DocumentIndexer {
public:
    /**
     * @brief Constructor.
     */
    DocumentIndexer();

    /**
     * @brief Indexes a single document from a file path.
     * 
     * @param filePath Path to the text file to index
     * @return True if indexing was successful, false otherwise
     */
    bool indexDocument(const std::string& filePath);

    /**
     * @brief Indexes multiple documents from file paths.
     * 
     * @param filePaths Vector of file paths to index
     * @return Number of successfully indexed documents
     */
    int indexDocuments(const std::vector<std::string>& filePaths);

    /**
     * @brief Gets all indexed documents.
     * 
     * @return Vector of document pointers
     */
    const std::vector<std::shared_ptr<Document>>& getDocuments() const;

    /**
     * @brief Gets the total number of documents in the index.
     * 
     * @return Number of documents
     */
    size_t getDocumentCount() const;

    /**
     * @brief Clears all indexed documents.
     */
    void clear();

    /**
     * @brief Gets a document by its file path.
     * 
     * @param filePath The file path to search for
     * @return Pointer to document if found, nullptr otherwise
     */
    std::shared_ptr<Document> getDocument(const std::string& filePath) const;

private:
    std::vector<std::shared_ptr<Document>> documents;
    std::unique_ptr<Tokenizer> tokenizer;
    std::unique_ptr<StopWordRemover> stopWordRemover;
    
    /**
     * @brief Reads the content of a file.
     * 
     * @param filePath Path to the file
     * @return File content as string, empty if read fails
     */
    std::string readFile(const std::string& filePath) const;
    
    /**
     * @brief Processes tokens and builds term frequency map for a document.
     * 
     * @param tokens Vector of tokens
     * @return Term frequency map
     */
    std::unordered_map<std::string, int> buildTermFrequency(const std::vector<std::string>& tokens) const;
};

#endif // DOCUMENTINDEXER_H

