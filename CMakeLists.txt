cmake_minimum_required(VERSION 3.26)
project(B+TreeImplementation)

set(CMAKE_CXX_STANDARD 14)

include_directories(src)

add_executable(B+TreeImplementation
        src/HeaderBuffer.cpp
        src/HeaderBuffer.h
        src/main.cpp
        src/Record.cpp
        src/Record.h
        src/RecordBuffer.cpp
        src/RecordBuffer.h
        src/RecordFile.cpp
        src/RecordFile.h
        src/StateDatabase.cpp
        src/StateDatabase.h
        src/StateExtrema.h
        src/BlockBuffer.cpp
        src/BlockBuffer.h
        src/BTreeFile.cpp
        src/BTreeFile.h
        src/BTreeNode.cpp
        src/BTreeNode.h
        src/BTreeIndexBuffer.cpp
        src/BTreeIndexBuffer.h)
