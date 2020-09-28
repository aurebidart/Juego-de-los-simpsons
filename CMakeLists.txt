cmake_minimum_required(VERSION 3.17)
project(untitled3)

set(CMAKE_CXX_STANDARD 14)
find_package(SFML 2.5 COMPONENTS system window graphics)

add_executable(prueba main.cpp homer.cpp homer.h Comida.cpp Comida.h Corazon.cpp Corazon.h Puntuaciones.cpp Puntuaciones.h basura.cpp basura.h)
target_link_libraries(prueba  sfml-system sfml-window sfml-graphics sfml-audio)