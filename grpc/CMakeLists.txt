cmake_minimum_required(VERSION 2.8)

project(ftext-grpc)

set (ftext-grpc_VERSION_MAJOR 0)
set (ftext-grpc_VERSION_MINOR 1)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Protobuf REQUIRED)

include_directories("${CMAKE_SOURCE_DIR}/include")
find_library(fasttext-shared NAMES libfasttext.so PATHS /usr/lib /usr/local/lib)
find_library(grpc-shared NAMES grpc)
find_library(grpcpp-shared NAMES grpc++)

add_subdirectory(src)
