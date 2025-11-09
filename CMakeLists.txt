cmake_minimum_required(VERSION 3.10)
project(AcoesImobiliarias LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(raylib REQUIRED) # espere que o sistema tenha raylib instalado / encontrado

add_executable(AcoesImobiliarias
    src/main.cpp
    src/Game.cpp
)

target_include_directories(AcoesImobiliarias PRIVATE src)
target_link_libraries(AcoesImobiliarias PRIVATE raylib)