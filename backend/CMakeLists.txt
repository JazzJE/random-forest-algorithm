cmake_minimum_required(VERSION 3.15)
project(backend)

# C++17 standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Output executable to /bin directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Collect all source files automatically from src/
file(GLOB_RECURSE SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

# Create executable
add_executable(backend ${SRC_FILES})

# Include directories for header files
target_include_directories(backend PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/external
)

# Optional: copy frontend dist folder (if building full stack)
add_custom_command(TARGET backend POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/../frontend/dist
        ${CMAKE_BINARY_DIR}/frontend_dist
)