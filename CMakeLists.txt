cmake_minimum_required(VERSION 3.12)
project(Zad2)

include_directories(include)

set(SOURCE_FILES
        src/main.cpp
        src/COM.cpp
        src/Sender.cpp
        src/checksum_algorithms.cpp
        src/Receiver.cpp include/Exceptions.h)

add_executable(Zad2 ${SOURCE_FILES})

