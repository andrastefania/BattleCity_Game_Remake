cmake_minimum_required(VERSION 3.15)

# Numele proiectului
project(MyProject)

# Setarea standardului C++
set(CMAKE_CXX_STANDARD 17)

# Add the libs folder (for json.hpp) to the include directories
include_directories(${CMAKE_SOURCE_DIR}/libs)

# Adăugăm subdirectoarele
add_subdirectory(server)
add_subdirectory(client)
