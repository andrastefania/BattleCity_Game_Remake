# Adăugăm un executabil pentru server
add_executable(server
    main.cpp
    server.cpp
    server.h
)

# Adăugăm include directories dacă este necesar
target_include_directories(server PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Adăugăm flaguri (opțional, pentru debug/release)
target_compile_options(server PRIVATE -Wall -Wextra)
