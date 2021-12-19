find_package(GTest 1.8 REQUIRED)
add_library(fakes
    SHARED
    fakes/FakeDevice.cpp)

target_include_directories(fakes
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/tests/fakes>
    )

set_target_properties(fakes
    PROPERTIES
    CXX_STANDARD 17
    )
target_compile_options(fakes PRIVATE
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Werror>
    )


add_executable(AnagramTest AnagramTest.cpp)
target_link_libraries(AnagramTest hemiola GTest::GTest GTest::Main -no-pie)
gtest_add_tests(TARGET AnagramTest)
set_target_properties(AnagramTest
    PROPERTIES
    CXX_STANDARD 17
    )

add_executable(KeyboardEventTest KeyboardEventTest.cpp)
target_link_libraries(KeyboardEventTest hemiola GTest::GTest GTest::Main -no-pie)
gtest_add_tests(TARGET KeyboardEventTest)
set_target_properties(KeyboardEventTest
    PROPERTIES
    CXX_STANDARD 17
    )
