# C++ Search Engine with Web UI

A full-featured search engine built in C++ with a modern web interface. This project implements a complete information retrieval system that indexes text documents, processes search queries using natural language processing techniques, and returns ranked results based on TF-IDF (Term Frequency-Inverse Document Frequency) scoring. The application features a Batman-themed responsive web UI, RESTful API endpoints, and cross-platform support for macOS, Linux, and Windows.

## Features

- **Document Indexing**: Automatically indexes text files from the `data/` directory
- **Tokenization**: Breaks text into words, handling punctuation and case normalization
- **Stop Word Removal**: Filters out common words that don't contribute to search relevance
- **TF-IDF Ranking**: Uses advanced ranking algorithm to score document relevance
- **Modern Web UI**: Beautiful, responsive web interface with real-time search results
- **REST API**: HTTP server exposing search functionality via REST endpoints
- **Cross-Platform**: Works on macOS, Linux, and Windows

## Prerequisites

- **C++ Compiler** with C++17 support (Clang 10.0+, GCC 7.0+, or MSVC 2017+)
- **CMake**: Version 3.16 or higher
- **Web Browser**: Any modern browser for accessing the web interface

### Quick Setup

**macOS:**
```bash
xcode-select --install
brew install cmake
```

**Linux (Ubuntu/Debian):**
```bash
sudo apt-get update
sudo apt-get install -y build-essential cmake
```

**Windows:**
- Install [Visual Studio Community](https://visualstudio.microsoft.com/downloads/) with "Desktop development with C++" workload
- Or install MinGW-w64 and CMake

## Building the Project

### macOS/Linux

```bash
chmod +x build.sh
./build.sh
```

Or manually:
```bash
mkdir build && cd build
cmake ..
cmake --build .
```

### Windows

```cmd
build.bat
```

Or manually:
```cmd
mkdir build && cd build
cmake ..
cmake --build . --config Release
```

**Output:** `build/bin/SearchEngine` (or `SearchEngine.exe` on Windows)

## Running the Application

1. **Start the server:**
   ```bash
   ./build/bin/SearchEngine
   # Or with custom port:
   ./build/bin/SearchEngine 3000
   ```

2. **Access the web interface:**
   - Open browser and navigate to `http://localhost:8080`
   - Enter search query and click "Search"

3. **Add your own documents:**
   - Place `.txt` files in the `data/` directory
   - Restart the server to index new files

## API Documentation

### Endpoints

#### `GET /api/search?q=<query>`
Performs a search query and returns JSON results.

**Example:**
```bash
curl "http://localhost:8080/api/search?q=artificial%20intelligence"
```

**Response:**
```json
{
  "results": [
    {
      "fileName": "Artificial_Intelligence.txt",
      "filePath": "./data/Artificial_Intelligence.txt",
      "score": 0.1234
    }
  ]
}
```

#### `GET /api/document?path=<filepath>`
Retrieves the full content of a document.

**Example:**
```bash
curl "http://localhost:8080/api/document?path=./data/Artificial_Intelligence.txt"
```

#### `GET /api/status`
Returns server status information.

**Response:**
```json
{
  "documentCount": 5
}
```

## Architecture

### Core Components

1. **Tokenizer**: Splits text into tokens, handles punctuation and case normalization
2. **StopWordRemover**: Filters common English stop words
3. **DocumentIndexer**: Reads and indexes text files, builds term frequency maps
4. **TFIDFCalculator**: Calculates TF-IDF scores for ranking
5. **SearchEngine**: Orchestrates indexing and searching operations
6. **Server**: HTTP server that exposes search engine as REST API

### Project Structure

```
SearchEngine/
├── CMakeLists.txt
├── build.sh / build.bat
├── include/
│   ├── core/          # Core search engine components
│   └── server/        # HTTP server
├── src/
│   ├── core/          # Implementation files
│   └── server/
├── web/                # Web interface (HTML, CSS, JS)
└── data/               # Text documents to index
```

## How TF-IDF Works

**Term Frequency (TF):** Measures how frequently a term appears in a document.
```
TF(term, document) = (Number of times term appears) / (Total terms in document)
```

**Inverse Document Frequency (IDF):** Measures how rare a term is across all documents.
```
IDF(term) = log(Total documents / Documents containing term)
```

**TF-IDF Score:** Combines both metrics to rank document relevance.
```
TF-IDF(term, document) = TF(term, document) × IDF(term)
```

### Search Algorithm

1. **Query Processing:** Tokenize query, remove stop words, normalize case
2. **Document Scoring:** Calculate TF-IDF score for each query term in each document
3. **Result Ranking:** Sort documents by total score (descending)

## Troubleshooting

**Port already in use:**
- Use a different port: `./build/bin/SearchEngine 3000`
- Or stop the process: `lsof -ti:8080 | xargs kill` (macOS/Linux)

**No search results:**
- Ensure `.txt` files exist in `data/` directory
- Restart server after adding new files
- Check that query terms aren't all stop words

**Build errors:**
- Verify CMake version: `cmake --version` (needs 3.16+)
- Ensure C++17 compiler is installed
- Build from project root directory

## License

This project is provided as an educational example. Feel free to modify and extend it for your own use.

---

**Enjoy searching!** 🔍
