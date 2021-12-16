cmake_minimum_required(VERSION 3.12)

project(hemiola VERSION 0.0.1)

set(PROJECT_HOMEPAGE_URL "https://github.com/erichlf/Hemiola")

option(BUILD_SHARED_LIBS "Build as shared library" ON)

add_library(hemiola
    SHARED
    src/Anagram.cpp)

target_include_directories(hemiola
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )

set_target_properties(hemiola
    PROPERTIES
    CXX_STANDARD 17
    )

find_package(GTest 1.8)

if((TARGET GTest::GTest) AND (TARGET GTest::Main))
    include(CTest)
endif()

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()

include(GNUInstallDirs)
