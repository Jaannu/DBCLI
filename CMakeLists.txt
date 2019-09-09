cmake_minimum_required(VERSION 3.14)
project(BaseDatosVirtual)

set(CMAKE_CXX_STANDARD 14)

add_executable(BaseDatosVirtual main.cpp DataBase.cpp DataBase.h Table.cpp Table.h Structs.h)