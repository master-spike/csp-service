cmake_minimum_required(VERSION 3.20)

project(csp-service)

add_subdirectory(thirdparty/doctest)
add_subdirectory(thirdparty/json)
add_subdirectory(thirdparty/cpp-httplib)

include(CTest)

add_library(csplib INTERFACE)

target_compile_features(csplib INTERFACE cxx_std_20)

target_include_directories(csplib INTERFACE include)

add_subdirectory(test)
