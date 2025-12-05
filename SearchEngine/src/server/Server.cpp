#include "server/Server.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>
#include <algorithm>
#include <iomanip>
#include <cstring>
#include <map>

// Simple HTTP server implementation
#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif

Server::Server(int port) : port(port), running(false) {
#ifdef _WIN32
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif
}

bool Server::start() {
    if (running) {
        return false;
    }
    
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        std::cerr << "Error creating socket" << std::endl;
        return false;
    }
    
    // Set socket options
    int opt = 1;
#ifdef _WIN32
    setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt));
#else
    setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
#endif
    
    sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);
    
    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Error binding socket to port " << port << std::endl;
#ifdef _WIN32
        closesocket(serverSocket);
#else
        close(serverSocket);
#endif
        return false;
    }
    
    if (listen(serverSocket, 10) < 0) {
        std::cerr << "Error listening on socket" << std::endl;
#ifdef _WIN32
        closesocket(serverSocket);
#else
        close(serverSocket);
#endif
        return false;
    }
    
    running = true;
    std::cout << "Server started on http://localhost:" << port << std::endl;
    std::cout << "Press Ctrl+C to stop the server" << std::endl;
    
    // Auto-index sample files
    std::vector<std::string> sampleFiles;
    std::filesystem::path dataDir;
    
    // Try multiple locations for data directory
    std::vector<std::filesystem::path> searchPaths = {
        std::filesystem::current_path() / "data",
        std::filesystem::current_path().parent_path().parent_path() / "data",
        std::filesystem::current_path().parent_path() / "data"
    };
    
    for (const auto& path : searchPaths) {
        if (std::filesystem::exists(path) && std::filesystem::is_directory(path)) {
            dataDir = path;
            break;
        }
    }
    
    if (!dataDir.empty()) {
        for (const auto& entry : std::filesystem::directory_iterator(dataDir)) {
            if (entry.is_regular_file() && entry.path().extension() == ".txt") {
                sampleFiles.push_back(entry.path().string());
            }
        }
    }
    if (!sampleFiles.empty()) {
        int indexed = searchEngine.indexDocuments(sampleFiles);
        std::cout << "Auto-indexed " << indexed << " sample files" << std::endl;
    }
    
    while (running) {
        sockaddr_in clientAddr;
        socklen_t clientLen = sizeof(clientAddr);
        int clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &clientLen);
        
        if (clientSocket < 0) {
            if (running) {
                std::cerr << "Error accepting connection" << std::endl;
            }
            continue;
        }
        
        // Read request
        char buffer[4096] = {0};
        int bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
        
        if (bytesRead > 0) {
            std::string request(buffer);
            std::istringstream requestStream(request);
            std::string method, path, protocol;
            requestStream >> method >> path >> protocol;
            
            std::string response;
            
            // Handle routes
            if (path == "/" || path == "/index.html") {
                response = serveStaticFile("web/index.html");
                if (response.empty()) {
                    response = "HTTP/1.1 404 Not Found\r\n\r\n";
                } else {
                    response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: " + 
                              std::to_string(response.length()) + "\r\n\r\n" + response;
                }
            } else if (path == "/style.css") {
                response = serveStaticFile("web/style.css");
                if (response.empty()) {
                    response = "HTTP/1.1 404 Not Found\r\n\r\n";
                } else {
                    response = "HTTP/1.1 200 OK\r\nContent-Type: text/css\r\nContent-Length: " + 
                              std::to_string(response.length()) + "\r\n\r\n" + response;
                }
            } else if (path == "/script.js") {
                response = serveStaticFile("web/script.js");
                if (response.empty()) {
                    response = "HTTP/1.1 404 Not Found\r\n\r\n";
                } else {
                    response = "HTTP/1.1 200 OK\r\nContent-Type: application/javascript\r\nContent-Length: " + 
                              std::to_string(response.length()) + "\r\n\r\n" + response;
                }
            } else if (path.find(".png") != std::string::npos || path.find(".jpg") != std::string::npos || 
                       path.find(".jpeg") != std::string::npos || path.find(".gif") != std::string::npos ||
                       path.find(".svg") != std::string::npos || path.find(".webp") != std::string::npos) {
                // Serve image files
                std::string imagePath = "web" + path;
                response = serveStaticFile(imagePath);
                if (response.empty()) {
                    response = "HTTP/1.1 404 Not Found\r\n\r\n";
                } else {
                    std::string contentType = "image/png";
                    if (path.find(".jpg") != std::string::npos || path.find(".jpeg") != std::string::npos) {
                        contentType = "image/jpeg";
                    } else if (path.find(".gif") != std::string::npos) {
                        contentType = "image/gif";
                    } else if (path.find(".svg") != std::string::npos) {
                        contentType = "image/svg+xml";
                    } else if (path.find(".webp") != std::string::npos) {
                        contentType = "image/webp";
                    }
                    response = "HTTP/1.1 200 OK\r\nContent-Type: " + contentType + "\r\nAccess-Control-Allow-Origin: *\r\nContent-Length: " + 
                              std::to_string(response.length()) + "\r\n\r\n" + response;
                }
            } else if (path.find("/api/search") == 0) {
                // Extract query parameter
                size_t queryPos = path.find("?q=");
                std::string query;
                if (queryPos != std::string::npos) {
                    query = path.substr(queryPos + 3);
                    query = urlDecode(query);
                }
                
                auto results = searchEngine.search(query, 20);
                std::string json = resultsToJson(results);
                
                response = "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nAccess-Control-Allow-Origin: *\r\nContent-Length: " + 
                          std::to_string(json.length()) + "\r\n\r\n" + json;
            } else if (path.find("/api/document") == 0) {
                // Extract file path parameter
                size_t pathPos = path.find("?path=");
                std::string filePath;
                if (pathPos != std::string::npos) {
                    filePath = path.substr(pathPos + 6);
                    filePath = urlDecode(filePath);
                }
                
                std::string content = readFileContent(filePath);
                if (content.empty()) {
                    response = "HTTP/1.1 404 Not Found\r\n\r\n";
                } else {
                    // Escape JSON
                    std::string escaped;
                    for (char c : content) {
                        if (c == '"') escaped += "\\\"";
                        else if (c == '\\') escaped += "\\\\";
                        else if (c == '\n') escaped += "\\n";
                        else if (c == '\r') escaped += "\\r";
                        else if (c == '\t') escaped += "\\t";
                        else escaped += c;
                    }
                    
                    std::string json = "{\"content\":\"" + escaped + "\"}";
                    response = "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nAccess-Control-Allow-Origin: *\r\nContent-Length: " + 
                              std::to_string(json.length()) + "\r\n\r\n" + json;
                }
            } else if (path == "/api/status") {
                std::string json = "{\"documentCount\":" + std::to_string(searchEngine.getDocumentCount()) + "}";
                response = "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nAccess-Control-Allow-Origin: *\r\nContent-Length: " + 
                          std::to_string(json.length()) + "\r\n\r\n" + json;
            } else if (path.find("/api/suggestions") == 0) {
                // Extract query parameter
                size_t queryPos = path.find("?q=");
                std::string query;
                if (queryPos != std::string::npos) {
                    query = path.substr(queryPos + 3);
                    query = urlDecode(query);
                }
                
                // Get suggestions based on query
                std::vector<std::string> suggestions = getSuggestions(query);
                std::string json = suggestionsToJson(suggestions);
                
                response = "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nAccess-Control-Allow-Origin: *\r\nContent-Length: " + 
                          std::to_string(json.length()) + "\r\n\r\n" + json;
            } else {
                response = "HTTP/1.1 404 Not Found\r\n\r\n";
            }
            
            send(clientSocket, response.c_str(), response.length(), 0);
        }
        
#ifdef _WIN32
        closesocket(clientSocket);
#else
        close(clientSocket);
#endif
    }
    
#ifdef _WIN32
    closesocket(serverSocket);
    WSACleanup();
#else
    close(serverSocket);
#endif
    
    return true;
}

void Server::stop() {
    running = false;
}

std::string Server::readFileContent(const std::string& filePath) const {
    std::ifstream file(filePath, std::ios::binary);
    if (!file.is_open()) {
        return "";
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

std::string Server::serveStaticFile(const std::string& path) const {
    // Try current directory first
    std::filesystem::path filePath = std::filesystem::current_path() / path;
    if (std::filesystem::exists(filePath)) {
        return readFileContent(filePath.string());
    }
    
    // Try parent directory (in case running from build/bin/)
    filePath = std::filesystem::current_path().parent_path().parent_path() / path;
    if (std::filesystem::exists(filePath)) {
        return readFileContent(filePath.string());
    }
    
    // Try two levels up (build/bin/)
    filePath = std::filesystem::current_path().parent_path() / path;
    if (std::filesystem::exists(filePath)) {
        return readFileContent(filePath.string());
    }
    
    return "";
}

std::string Server::resultsToJson(const std::vector<SearchResult>& results) const {
    std::ostringstream json;
    json << "{\"results\":[";
    
    for (size_t i = 0; i < results.size(); ++i) {
        const auto& result = results[i];
        json << "{";
        json << "\"fileName\":\"" << result.document->fileName << "\",";
        json << "\"filePath\":\"" << result.document->filePath << "\",";
        json << "\"score\":" << std::fixed << std::setprecision(4) << result.score;
        json << "}";
        if (i < results.size() - 1) {
            json << ",";
        }
    }
    
    json << "]}";
    return json.str();
}

std::string Server::urlDecode(const std::string& str) const {
    std::string result;
    for (size_t i = 0; i < str.length(); ++i) {
        if (str[i] == '%' && i + 2 < str.length()) {
            int value;
            std::istringstream hexStream(str.substr(i + 1, 2));
            hexStream >> std::hex >> value;
            result += static_cast<char>(value);
            i += 2;
        } else if (str[i] == '+') {
            result += ' ';
        } else {
            result += str[i];
        }
    }
    return result;
}

std::vector<std::string> Server::getSuggestions(const std::string& query) const {
    std::vector<std::string> suggestions;
    std::map<std::string, int> termCounts; // term -> frequency across all documents
    
    if (query.empty()) {
        return suggestions;
    }
    
    // Convert query to lowercase for case-insensitive matching
    std::string lowerQuery = query;
    std::transform(lowerQuery.begin(), lowerQuery.end(), lowerQuery.begin(), ::tolower);
    
    // Get all documents from the indexer
    const auto& documents = searchEngine.getDocuments();
    
    // Collect all unique terms that contain the query
    // Prioritize terms that start with the query
    for (const auto& doc : documents) {
        for (const auto& token : doc->tokens) {
            std::string lowerToken = token;
            std::transform(lowerToken.begin(), lowerToken.end(), lowerToken.begin(), ::tolower);
            
            // Check if token contains query and is longer than query
            if (lowerToken.find(lowerQuery) != std::string::npos && lowerToken.length() > lowerQuery.length()) {
                auto it = doc->termFrequency.find(token);
                if (it != doc->termFrequency.end()) {
                    // Boost terms that start with the query
                    int boost = (lowerToken.find(lowerQuery) == 0) ? 1000 : 1;
                    termCounts[token] += (it->second * boost);
                }
            }
        }
    }
    
    // Sort by frequency (descending) and take top 8 suggestions
    std::vector<std::pair<std::string, int>> sortedTerms(termCounts.begin(), termCounts.end());
    std::sort(sortedTerms.begin(), sortedTerms.end(), 
              [](const std::pair<std::string, int>& a, const std::pair<std::string, int>& b) {
                  return a.second > b.second;
              });
    
    // Extract suggestions (limit to 8)
    for (size_t i = 0; i < sortedTerms.size() && i < 8; ++i) {
        suggestions.push_back(sortedTerms[i].first);
    }
    
    return suggestions;
}

std::string Server::suggestionsToJson(const std::vector<std::string>& suggestions) const {
    std::ostringstream json;
    json << "{\"suggestions\":[";
    
    for (size_t i = 0; i < suggestions.size(); ++i) {
        // Escape JSON special characters
        std::string escaped = suggestions[i];
        size_t pos = 0;
        while ((pos = escaped.find('"', pos)) != std::string::npos) {
            escaped.replace(pos, 1, "\\\"");
            pos += 2;
        }
        pos = 0;
        while ((pos = escaped.find('\\', pos)) != std::string::npos) {
            escaped.replace(pos, 1, "\\\\");
            pos += 2;
        }
        
        json << "\"" << escaped << "\"";
        if (i < suggestions.size() - 1) {
            json << ",";
        }
    }
    
    json << "]}";
    return json.str();
}

Server::~Server() {
    stop();
#ifdef _WIN32
    WSACleanup();
#endif
}

