cmake_minimum_required(VERSION 3.16)
project(AcoesImobiliarias)

# Forçar compilação C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Encontrar Raylib
find_package(raylib REQUIRED)

# Adicionar executável com TODOS os arquivos
add_executable(AcoesImobiliarias 
    src/main.cpp
    src/Player.cpp
)

# Linkar com Raylib e biblioteca C++
target_link_libraries(AcoesImobiliarias raylib stdc++)

# Para Windows
if(WIN32)
    target_compile_options(AcoesImobiliarias PRIVATE -static)
    target_link_libraries(AcoesImobiliarias -static)
endif()