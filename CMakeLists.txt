cmake_minimum_required(VERSION 3.12)

project(hemiola VERSION 0.0.1)

set(PROJECT_HOMEPAGE_URL "https://github.com/erichlf/Hemiola")

option(BUILD_SHARED_LIBS "Build as shared library" ON)

include(cmake/External.cmake)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

##################  create a hemiola library ##################
add_library(hemiolalib
    SHARED
    src/Hemiola.cpp
    src/HID.cpp
    src/Keyboard.cpp
    src/KeyChords.cpp
    src/KeyTable.cpp
    src/KeyboardEvents.cpp
    src/Logger.cpp
    src/OutputHID.cpp
    src/USBHID.cpp
    )

add_dependencies(hemiolalib ${EXTERNAL_DEPENDENCIES})

target_link_libraries(hemiolalib ${EXTERNAL_LIBS})

target_include_directories(hemiolalib
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    ${EXTERNAL_INCLUDE_DIRS}
    )

set_target_properties(hemiolalib
    PROPERTIES
    CXX_STANDARD 17
    )
target_compile_options(hemiolalib PRIVATE
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Werror -Wno-psabi>
    )

##################   create the executable for hemiola ##################
add_executable(hemiola src/hemiola.cpp)

target_link_libraries(hemiola PRIVATE hemiolalib)

target_include_directories(hemiolalib
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )

set_target_properties(hemiola
    PROPERTIES
    CXX_STANDARD 20
    )
target_compile_options(hemiola PRIVATE
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Werror -Wconversion -Wno-psabi>
    )

find_package(GTest 1.8)

if((TARGET GTest::GTest) AND (TARGET GTest::Main))
    include(CTest)
endif()

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()

include(GNUInstallDirs)
