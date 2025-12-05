#include "server/Server.h"
#include <iostream>
#include <csignal>

Server* g_server = nullptr;

void signalHandler(int signal) {
    if (g_server) {
        std::cout << "\nShutting down server..." << std::endl;
        g_server->stop();
    }
    exit(0);
}

int main(int argc, char *argv[]) {
    int port = 8080;
    
    // Parse command line arguments
    if (argc > 1) {
        try {
            port = std::stoi(argv[1]);
        } catch (...) {
            std::cerr << "Invalid port number. Using default port 8080." << std::endl;
        }
    }
    
    Server server(port);
    g_server = &server;
    
    // Register signal handlers
    signal(SIGINT, signalHandler);
#ifndef _WIN32
    signal(SIGTERM, signalHandler);
#endif
    
    // Start server
    if (!server.start()) {
        std::cerr << "Failed to start server" << std::endl;
        return 1;
    }
    
    return 0;
}

