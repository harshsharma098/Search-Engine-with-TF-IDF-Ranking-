cmake_minimum_required(VERSION 3.16)
project(SearchEngine VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(include)

# Source files
set(CORE_SOURCES
    src/core/Tokenizer.cpp
    src/core/StopWordRemover.cpp
    src/core/DocumentIndexer.cpp
    src/core/TFIDFCalculator.cpp
    src/core/SearchEngine.cpp
)

set(SERVER_SOURCES
    src/server/main.cpp
    src/server/Server.cpp
)

# Header files
set(HEADERS
    include/core/Tokenizer.h
    include/core/StopWordRemover.h
    include/core/DocumentIndexer.h
    include/core/TFIDFCalculator.h
    include/core/SearchEngine.h
    include/server/Server.h
)

# Create executable
add_executable(${PROJECT_NAME}
    ${CORE_SOURCES}
    ${SERVER_SOURCES}
    ${HEADERS}
)

# Link system libraries
if(WIN32)
    target_link_libraries(${PROJECT_NAME} ws2_32)
else()
    # Unix/Linux/macOS - no additional libraries needed for basic sockets
endif()

# Link filesystem library if needed (GCC < 9, or explicitly required)
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS "9.0")
    target_link_libraries(${PROJECT_NAME} stdc++fs)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS "9.0")
    target_link_libraries(${PROJECT_NAME} c++fs)
endif()

# Set output directory
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

