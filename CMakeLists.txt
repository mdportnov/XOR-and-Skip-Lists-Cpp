cmake_minimum_required(VERSION 3.16)
project(structures)

set(CMAKE_CXX_STANDARD 14)

add_executable(structures main.cpp)
add_executable(XOR XORDoublyLinkedList.cpp)
add_executable(skipList skipList.cpp)