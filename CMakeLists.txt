cmake_minimum_required(VERSION 3.16)

project(AdvPT VERSION 1.0
              DESCRIPTION "Escaping the grid"
              LANGUAGES CXX)

add_executable(gridtest Tile.cpp MyGrid.cpp gridtest.cpp)
target_compile_features(gridtest PRIVATE cxx_std_20)
target_compile_options(gridtest PRIVATE -Wall -Wextra -pedantic -Werror)

add_executable(escapetest Tile.cpp MyGrid.cpp escape.cpp escapetest.cpp)
target_compile_features(escapetest PRIVATE cxx_std_20)
target_compile_options(escapetest PRIVATE -Wall -Wextra -pedantic -Werror)
