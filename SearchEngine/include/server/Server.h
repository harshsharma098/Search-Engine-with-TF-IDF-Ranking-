#ifndef SERVER_H
#define SERVER_H

#include "core/SearchEngine.h"
#include <string>
#include <vector>

/**
 * @brief HTTP server that exposes the search engine as a REST API.
 */
class Server {
public:
    /**
     * @brief Constructor.
     * 
     * @param port Port number to listen on (default: 8080)
     */
    Server(int port = 8080);

    /**
     * @brief Starts the HTTP server.
     * 
     * @return True if server started successfully, false otherwise
     */
    bool start();

    /**
     * @brief Stops the HTTP server.
     */
    void stop();

    /**
     * @brief Destructor.
     */
    ~Server();

private:
    int port;
    SearchEngine searchEngine;
    bool running;
    
    /**
     * @brief Reads a file and returns its content.
     */
    std::string readFileContent(const std::string& filePath) const;
    
    /**
     * @brief Serves static files (HTML, CSS, JS).
     */
    std::string serveStaticFile(const std::string& path) const;
    
    /**
     * @brief Converts search results to JSON.
     */
    std::string resultsToJson(const std::vector<SearchResult>& results) const;
    
    /**
     * @brief URL decodes a string.
     */
    std::string urlDecode(const std::string& str) const;
    
    /**
     * @brief Gets search suggestions based on a query prefix.
     * 
     * @param query The query prefix to match
     * @return Vector of suggestion strings
     */
    std::vector<std::string> getSuggestions(const std::string& query) const;
    
    /**
     * @brief Converts suggestions to JSON.
     * 
     * @param suggestions Vector of suggestion strings
     * @return JSON string
     */
    std::string suggestionsToJson(const std::vector<std::string>& suggestions) const;
};

#endif // SERVER_H

