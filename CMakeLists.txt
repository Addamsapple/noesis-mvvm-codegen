cmake_minimum_required(VERSION 3.14)

project(noesis-mvvm-codegen VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(MVVM_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/source/BaseModelCollection.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/source/BaseProperty.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/source/Cache.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/source/Model.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/source/PropertyList.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/source/ViewModel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/source/ViewModelFactory.cpp
)
set(MVVM_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

set(CODEGEN_DIR ${CMAKE_CURRENT_SOURCE_DIR}/codegen)

set(NOESIS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/noesis)

option(BUILD_TESTS "Build tests" ON)
option(BUILD_SAMPLES "Build samples" OFF)

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

if(BUILD_SAMPLES)
    # add_subdirectory(samples)
    # message(STATUS "Samples not yet implemented")
endif()
