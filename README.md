# C++ Search Engine with Web UI

A full-featured search engine built in C++ with a modern web interface. This project implements a complete information retrieval system that indexes text documents, processes search queries using natural language processing techniques, and returns ranked results based on TF-IDF (Term Frequency-Inverse Document Frequency) scoring. The application features a Batman-themed responsive web UI, RESTful API endpoints, and cross-platform support for macOS, Linux, and Windows.

## Table of Contents

- [Features](#features)x
- [System Requirements](#system-requirements)
- [Prerequisites](#prerequisites)
- [Installation](#installation)
- [Building the Project](#building-the-project)
- [Running the Application](#running-the-application)
- [Usage Guide](#usage-guide)
- [API Documentation](#api-documentation)
- [Architecture](#architecture)
- [Troubleshooting](#troubleshooting)

## Features

- **Document Indexing**: Automatically indexes text files from the `data/` directory
- **Tokenization**: Breaks text into words, handling punctuation and case normalization
- **Stop Word Removal**: Filters out common words that don't contribute to search relevance
- **TF-IDF Ranking**: Uses advanced ranking algorithm to score document relevance
- **Modern Web UI**: Beautiful, responsive web interface inspired by Google/Yahoo with:
  - Clean, minimalist search interface
  - Real-time search results
  - Document preview modal
  - Mobile-responsive design
  - "I'm Feeling Lucky" feature
- **REST API**: HTTP server exposing search functionality via REST endpoints
- **Cross-Platform**: Works on macOS, Linux, and Windows
- **Modular Architecture**: Well-organized classes for maintainability and extensibility

## System Requirements

### Minimum Requirements

- **Operating System**: 
  - macOS 10.15 (Catalina) or later
  - Linux (Ubuntu 18.04+, Debian 10+, or equivalent)
  - Windows 10 or later
- **RAM**: 512 MB minimum (1 GB recommended)
- **Disk Space**: 50 MB for the application and dependencies
- **Network**: Local network access (for web interface)

### Software Requirements

- **C++ Compiler** with C++17 support:
  - macOS: Clang 10.0+ (included with Xcode Command Line Tools)
  - Linux: GCC 7.0+ or Clang 9.0+
  - Windows: MSVC 2017+ (Visual Studio) or MinGW-w64 8.0+
- **CMake**: Version 3.16 or higher
- **Web Browser**: Any modern browser (Chrome, Firefox, Safari, Edge) for accessing the web interface

## Prerequisites

### macOS

#### Option 1: Using Xcode Command Line Tools (Recommended)

1. **Install Xcode Command Line Tools:**
   ```bash
   xcode-select --install
   ```
   This installs:
   - Clang compiler (C++17 support)
   - Make
   - Git
   - Other development tools

2. **Install CMake:**
   ```bash
   # Using Homebrew (if you have it)
   brew install cmake
   
   # Or download from https://cmake.org/download/
   ```

3. **Verify Installation:**
   ```bash
   clang --version    # Should show Clang version 10.0 or higher
   cmake --version    # Should show CMake 3.16 or higher
   ```

#### Option 2: Using Homebrew

1. **Install Homebrew** (if not already installed):
   ```bash
   /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
   ```

2. **Install required packages:**
   ```bash
   brew install cmake
   ```

3. **Verify Installation:**
   ```bash
   cmake --version
   ```

### Linux (Ubuntu/Debian)

1. **Update package list:**
   ```bash
   sudo apt-get update
   ```

2. **Install build essentials and CMake:**
   ```bash
   sudo apt-get install -y build-essential cmake
   ```
   
   This installs:
   - GCC compiler
   - Make
   - CMake
   - Other essential build tools

3. **Verify Installation:**
   ```bash
   gcc --version      # Should show GCC 7.0 or higher
   cmake --version    # Should show CMake 3.16 or higher
   ```

### Linux (Other Distributions)

#### Fedora/RHEL/CentOS:
```bash
sudo dnf install gcc-c++ cmake make
# or
sudo yum install gcc-c++ cmake make
```

#### Arch Linux:
```bash
sudo pacman -S base-devel cmake
```

### Windows

#### Option 1: Using Visual Studio (Recommended)

1. **Download and Install Visual Studio:**
   - Download [Visual Studio Community](https://visualstudio.microsoft.com/downloads/) (free)
   - During installation, select:
     - "Desktop development with C++" workload
     - "CMake tools for Windows" component

2. **Verify Installation:**
   - Open "Developer Command Prompt for VS"
   - Run:
     ```cmd
     cl
     cmake --version
     ```

#### Option 2: Using MinGW-w64

1. **Install MinGW-w64:**
   - Download from [MinGW-w64](https://www.mingw-w64.org/downloads/)
   - Or use [MSYS2](https://www.msys2.org/) which includes MinGW-w64:
     ```bash
     pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-cmake
     ```

2. **Install CMake:**
   - Download from [CMake website](https://cmake.org/download/)
   - Add CMake to your PATH

3. **Verify Installation:**
   ```cmd
   gcc --version
   cmake --version
   ```

## Installation

### Step 1: Clone or Download the Project

If using Git:
```bash
git clone <repository-url>
cd SearchEngine
```

Or download and extract the project to a directory of your choice.

### Step 2: Verify Project Structure

Ensure you have the following structure:
```
SearchEngine/
├── CMakeLists.txt
├── build.sh (macOS/Linux)
├── build.bat (Windows)
├── include/
├── src/
├── web/
└── data/
```

### Step 3: Verify Prerequisites

Run these commands to verify your setup:

**macOS/Linux:**
```bash
clang --version    # or gcc --version
cmake --version
```

**Windows:**
```cmd
cl              # Visual Studio
# or
gcc --version   # MinGW
cmake --version
```

## Building the Project

### macOS/Linux

#### Using the Build Script (Recommended)

1. **Make the script executable:**
   ```bash
   chmod +x build.sh
   ```

2. **Run the build script:**
   ```bash
   ./build.sh
   ```

   The script will:
   - Create a `build/` directory
   - Configure CMake
   - Compile the project
   - Place the executable in `build/bin/SearchEngine`

#### Manual Build

1. **Create build directory:**
   ```bash
   mkdir build
   cd build
   ```

2. **Configure CMake:**
   ```bash
   cmake ..
   ```

3. **Build the project:**
   ```bash
   cmake --build .
   # or
   make
   ```

4. **Verify build:**
   ```bash
   ls -la bin/SearchEngine
   ```

### Windows

#### Using the Build Script

1. **Run the batch file:**
   ```cmd
   build.bat
   ```

   The script will:
   - Create a `build\` directory
   - Configure CMake
   - Compile the project
   - Place the executable in `build\bin\SearchEngine.exe`

#### Manual Build (Visual Studio)

1. **Create build directory:**
   ```cmd
   mkdir build
   cd build
   ```

2. **Configure CMake:**
   ```cmd
   cmake ..
   ```

3. **Build the project:**
   ```cmd
   cmake --build . --config Release
   ```

   Or open the generated `.sln` file in Visual Studio and build from there.

#### Manual Build (MinGW)

1. **Create build directory:**
   ```cmd
   mkdir build
   cd build
   ```

2. **Configure CMake:**
   ```cmd
   cmake -G "MinGW Makefiles" ..
   ```

3. **Build:**
   ```cmd
   cmake --build .
   ```

### Build Output

After successful build, you should have:
- **macOS/Linux**: `build/bin/SearchEngine`
- **Windows**: `build\bin\SearchEngine.exe`

## Running the Application

### Step 1: Start the Server

**macOS/Linux:**
```bash
./build/bin/SearchEngine
```

**Windows:**
```cmd
build\bin\SearchEngine.exe
```

**With custom port:**
```bash
./build/bin/SearchEngine 3000  # Start on port 3000
```

### Step 2: Verify Server is Running

You should see output like:
```
Server started on http://localhost:8080
Press Ctrl+C to stop the server
Auto-indexed 5 sample files
```

### Step 3: Access the Web Interface

1. **Open your web browser**
2. **Navigate to:** `http://localhost:8080`
3. **You should see:**
   - The search engine homepage
   - A search box in the center
   - Status showing number of indexed documents

### Step 4: Perform a Search

1. **Enter a search query** in the search box
2. **Click "Search"** or press Enter
3. **View results** ranked by relevance
4. **Click on any result** to view the full document content

## Usage Guide

### Basic Search

1. Type your search query (e.g., "artificial intelligence")
2. Press Enter or click "Search"
3. Browse through ranked results
4. Click on a result to view the full document

### I'm Feeling Lucky

- Click "I'm Feeling Lucky" to go directly to the top-ranked result
- Useful when you're confident the first result is what you need

### Adding Your Own Documents

1. **Place text files** in the `data/` directory
2. **Ensure files have `.txt` extension**
3. **Restart the server** to index new files
4. **Search for content** from your documents

### Understanding Search Results

- **Results are ranked** by TF-IDF relevance score
- **Higher scores** indicate better relevance
- **Scores are displayed** for transparency
- **File paths** show document locations

### Keyboard Shortcuts

- **Enter**: Perform search
- **Escape**: Close document preview modal
- **Click outside modal**: Close document preview

## API Documentation

The server exposes REST API endpoints for programmatic access:

### Endpoints

#### `GET /`
Serves the main web interface (HTML page).

**Response:** HTML content

#### `GET /api/search?q=<query>`
Performs a search query and returns JSON results.

**Parameters:**
- `q` (required): URL-encoded search query

**Response:**
```json
{
  "results": [
    {
      "fileName": "sample1.txt",
      "filePath": "./data/sample1.txt",
      "score": 0.1234
    }
  ]
}
```

**Example:**
```bash
curl "http://localhost:8080/api/search?q=artificial%20intelligence"
```

#### `GET /api/document?path=<filepath>`
Retrieves the full content of a document.

**Parameters:**
- `path` (required): URL-encoded file path

**Response:**
```json
{
  "content": "Full document content here..."
}
```

**Example:**
```bash
curl "http://localhost:8080/api/document?path=./data/sample1.txt"
```

#### `GET /api/status`
Returns server status information.

**Response:**
```json
{
  "documentCount": 5
}
```

**Example:**
```bash
curl "http://localhost:8080/api/status"
```

### Using the API with JavaScript

```javascript
// Search
fetch('/api/search?q=' + encodeURIComponent('your query'))
  .then(response => response.json())
  .then(data => console.log(data));

// Get document
fetch('/api/document?path=' + encodeURIComponent('./data/sample1.txt'))
  .then(response => response.json())
  .then(data => console.log(data.content));

// Check status
fetch('/api/status')
  .then(response => response.json())
  .then(data => console.log('Documents:', data.documentCount));
```

## Architecture

### Core Components

1. **Tokenizer** (`core/Tokenizer.h/cpp`)
   - Splits text into tokens (words)
   - Handles punctuation and case normalization
   - Filters out single-character tokens

2. **StopWordRemover** (`core/StopWordRemover.h/cpp`)
   - Maintains a set of common English stop words
   - Filters tokens to improve search quality

3. **DocumentIndexer** (`core/DocumentIndexer.h/cpp`)
   - Reads and indexes text files
   - Builds term frequency maps for each document
   - Manages document collection

4. **TFIDFCalculator** (`core/TFIDFCalculator.h/cpp`)
   - Calculates Term Frequency (TF) for terms in documents
   - Calculates Inverse Document Frequency (IDF) for terms
   - Computes TF-IDF scores for ranking

5. **SearchEngine** (`core/SearchEngine.h/cpp`)
   - Orchestrates indexing and searching operations
   - Processes queries and returns ranked results
   - Main interface for search functionality

6. **Server** (`server/Server.h/cpp`)
   - HTTP server that exposes search engine as REST API
   - Serves static web files (HTML, CSS, JavaScript)
   - Handles search requests and document retrieval

### Frontend

- **HTML/CSS/JavaScript**: Modern, responsive web interface
- **Google/Yahoo-inspired design**: Clean, minimalist search interface
- **Interactive features**: Search, document preview, status indicators

### Project Structure

```
SearchEngine/
├── CMakeLists.txt          # Build configuration
├── README.md               # This file
├── build.sh                # macOS/Linux build script
├── build.bat               # Windows build script
├── include/
│   ├── core/
│   │   ├── Tokenizer.h
│   │   ├── StopWordRemover.h
│   │   ├── DocumentIndexer.h
│   │   ├── TFIDFCalculator.h
│   │   └── SearchEngine.h
│   └── server/
│       └── Server.h
├── src/
│   ├── core/
│   │   ├── Tokenizer.cpp
│   │   ├── StopWordRemover.cpp
│   │   ├── DocumentIndexer.cpp
│   │   ├── TFIDFCalculator.cpp
│   │   └── SearchEngine.cpp
│   └── server/
│       ├── main.cpp
│       └── Server.cpp
├── web/
│   ├── index.html          # Main web interface
│   ├── style.css           # Styling
│   └── script.js           # Frontend JavaScript
└── data/
    ├── sample1.txt
    ├── sample2.txt
    ├── sample3.txt
    ├── sample4.txt
    └── sample5.txt
```

## How TF-IDF Works

**Term Frequency (TF):** Measures how frequently a term appears in a document, normalized by document length.

```
TF(term, document) = (Number of times term appears in document) / (Total terms in document)
```

**Inverse Document Frequency (IDF):** Measures how rare or common a term is across all documents.

```
IDF(term) = log(Total documents / Documents containing term)
```

**TF-IDF Score:** Combines both metrics to rank document relevance.

```
TF-IDF(term, document) = TF(term, document) × IDF(term)
```

Documents with higher TF-IDF scores are considered more relevant to the query.

### Search Algorithm

1. **Query Processing:**
   - Tokenize the search query
   - Remove stop words
   - Normalize case

2. **Document Scoring:**
   - For each document, calculate TF-IDF score for each query term
   - Sum scores for all query terms
   - Rank documents by total score

3. **Result Ranking:**
   - Sort documents by score (descending)
   - Return top N results (default: 20)

## Troubleshooting

### Build Errors

#### "CMake not found"
- **Solution**: Install CMake using the instructions in [Prerequisites](#prerequisites)
- **Verify**: Run `cmake --version`

#### "C++17 not supported"
- **Solution**: Update your compiler to a version that supports C++17
- **macOS**: Update Xcode Command Line Tools
- **Linux**: `sudo apt-get install gcc-7` or higher
- **Windows**: Update Visual Studio or MinGW

#### "Cannot find include files"
- **Solution**: Ensure you're building from the project root directory
- **Verify**: Check that `include/` and `src/` directories exist

#### "Linker errors on Windows"
- **Solution**: Ensure you're linking against `ws2_32.lib` (should be automatic)
- **Visual Studio**: Check that Windows SDK is installed

### Server Won't Start

#### "Error binding socket to port"
- **Cause**: Port 8080 is already in use
- **Solution**: 
  - Use a different port: `./build/bin/SearchEngine 3000`
  - Or stop the process using port 8080:
    ```bash
    # macOS/Linux
    lsof -ti:8080 | xargs kill
    # Windows
    netstat -ano | findstr :8080
    taskkill /PID <PID> /F
    ```

#### "Permission denied"
- **Cause**: Insufficient permissions to bind to port
- **Solution**: 
  - Use a port above 1024 (non-privileged)
  - Or run with appropriate permissions (not recommended)

#### "Server starts but web interface doesn't load"
- **Cause**: Web files not found
- **Solution**: 
  - Ensure `web/` directory exists in project root
  - Run server from project root directory
  - Check server console for error messages

### No Search Results

#### "No documents indexed"
- **Cause**: No `.txt` files in `data/` directory
- **Solution**: 
  - Add text files to `data/` directory
  - Ensure files have `.txt` extension
  - Restart the server

#### "Empty results for valid query"
- **Cause**: Query terms might be all stop words
- **Solution**: 
  - Try more specific search terms
  - Check that documents contain your search terms
  - Verify documents were indexed (check server startup messages)

### Web Interface Issues

#### "Page not loading"
- **Check**: Server is running (check console output)
- **Check**: Correct URL (`http://localhost:8080`)
- **Check**: Browser console for JavaScript errors (F12)
- **Check**: Firewall isn't blocking localhost

#### "Search button doesn't work"
- **Check**: Browser console for JavaScript errors
- **Check**: Server is responding to API requests
- **Solution**: Hard refresh page (Ctrl+F5 or Cmd+Shift+R)

#### "Results not displaying"
- **Check**: Browser console for errors
- **Check**: Network tab shows API requests
- **Check**: Server console for errors

### Performance Issues

#### "Slow search results"
- **Cause**: Large number of documents
- **Solution**: 
  - Limit results: Modify `maxResults` parameter
  - Optimize indexing (future enhancement)

#### "High memory usage"
- **Cause**: Large documents or many documents
- **Solution**: 
  - Process documents in batches
  - Consider index persistence (future enhancement)

## Platform-Specific Notes

### macOS

- **Tested on**: macOS 10.15 (Catalina) through macOS 14 (Sonoma)
- **Compiler**: Clang (included with Xcode Command Line Tools)
- **Dependencies**: None (uses standard POSIX sockets)
- **Notes**: 
  - Works out of the box after installing Xcode Command Line Tools
  - No additional libraries required

### Linux

- **Tested on**: Ubuntu 18.04+, Debian 10+, Fedora 32+
- **Compiler**: GCC or Clang
- **Dependencies**: `build-essential` package
- **Notes**: 
  - May need to install `build-essential` for compilation tools
  - Works on all major distributions

### Windows

- **Tested on**: Windows 10, Windows 11
- **Compiler**: MSVC 2017+ or MinGW-w64 8.0+
- **Dependencies**: Winsock2 (included with Windows)
- **Notes**: 
  - Visual Studio recommended for easiest setup
  - MinGW-w64 works but requires manual PATH configuration

## Sample Data

The `data/` directory contains sample text files covering topics like:
- Artificial Intelligence and Machine Learning
- Web Development
- Data Structures and Algorithms
- Software Engineering
- Database Systems

The server automatically indexes all `.txt` files in the `data/` directory on startup. You can add your own text files to this directory and restart the server to index them.

## Extending the Project

### Adding New Features

- **Stemming/Lemmatization**: Normalize word variations (e.g., "running" → "run")
- **Phrase Search**: Support for exact phrase matching
- **Boolean Operators**: AND, OR, NOT query support
- **Highlighting**: Highlight matching terms in document preview
- **Export Results**: Save search results to file
- **Index Persistence**: Save/load index to avoid re-indexing
- **Advanced UI Features**: Autocomplete, search suggestions, filters
- **Multi-threading**: Parallel document processing
- **Caching**: Cache search results for common queries

### Code Organization

The project follows a modular design:
- Core search functionality is separated from the web interface
- Each component has a single responsibility
- Easy to test individual components
- Can be extended without modifying existing code

## License

This project is provided as an educational example. Feel free to modify and extend it for your own use.

## Contributing

This is a demonstration project. Suggestions and improvements are welcome!

## Author

Created as an intermediate-level C++ project demonstrating:
- Modern C++ features (C++17)
- Web server development
- REST API design
- Information retrieval algorithms
- Software architecture and design patterns
- Cross-platform development

---

**Enjoy searching!** 🔍
