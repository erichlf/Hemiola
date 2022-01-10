find_package(GTest 1.8 REQUIRED)

add_subdirectory(fakes)

add_executable(AnagramTest AnagramTest.cpp)
target_link_libraries(AnagramTest fakes hemiolalib GTest::GTest GTest::Main -no-pie)
gtest_add_tests(TARGET AnagramTest)
set_target_properties(AnagramTest
    PROPERTIES
    CXX_STANDARD 17
    )

add_executable(KeyboardEventTest KeyboardEventTest.cpp)
target_link_libraries(KeyboardEventTest fakes hemiolalib GTest::GTest GTest::Main -no-pie)
gtest_add_tests(TARGET KeyboardEventTest)
set_target_properties(KeyboardEventTest
    PROPERTIES
    CXX_STANDARD 17
    )
