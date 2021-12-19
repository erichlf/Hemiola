cmake_minimum_required(VERSION 3.12)

project(hemiola VERSION 0.0.1)

set(PROJECT_HOMEPAGE_URL "https://github.com/erichlf/Hemiola")

option(BUILD_SHARED_LIBS "Build as shared library" ON)

include(ExternalProject)
find_package(Git REQUIRED)

set(EXTERNAL_INSTALL_LOCATION ${CMAKE_BINARY_DIR}/external)

ExternalProject_Add(nuspell
    PREFIX ${EXTERNAL_INSTALL_LOCATION}
    GIT_REPOSITORY https://github.com/nuspell/nuspell
    GIT_TAG "v5.0.1"
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${EXTERNAL_INSTALL_LOCATION}
    )

include_directories(${EXTERNAL_INSTALL_LOCATION}/include)
link_directories(${EXTERNAL_INSTALL_LOCATION}/lib)

add_library(hemiola
    SHARED
    src/Anagram.cpp
    src/Device.cpp
    src/KeyTable.cpp
    src/KeyboardEvents.cpp)

add_dependencies(hemiola nuspell)

add_library(Nuspell::nuspell SHARED IMPORTED)

target_link_libraries(hemiola ${EXTERNAL_INSTALL_LOCATION}/lib/libnuspell.so)

target_include_directories(hemiola
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )

set_target_properties(hemiola
    PROPERTIES
    CXX_STANDARD 17
    )
target_compile_options(hemiola PRIVATE
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Werror>
    )

find_package(GTest 1.8)

if((TARGET GTest::GTest) AND (TARGET GTest::Main))
    include(CTest)
endif()

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()

include(GNUInstallDirs)
