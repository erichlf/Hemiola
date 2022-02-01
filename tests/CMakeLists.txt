find_package(GTest 1.8 REQUIRED)

add_subdirectory(fakes)

add_executable(AnagrammerTest AnagrammerTest.cpp)
target_link_libraries(AnagrammerTest fakes hemiolalib GTest::GTest GTest::Main -no-pie)
gtest_add_tests(TARGET AnagrammerTest)
set_target_properties(AnagrammerTest
    PROPERTIES
    CXX_STANDARD 17
    )


add_executable(KeyTableTest KeyTableTest.cpp)
target_link_libraries(KeyTableTest fakes hemiolalib GTest::GTest GTest::Main -no-pie)
gtest_add_tests(TARGET KeyTableTest)
set_target_properties(KeyTableTest
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

add_executable(HemiolaTest HemiolaTest.cpp)
target_link_libraries(HemiolaTest fakes hemiolalib GTest::GTest GTest::Main -no-pie)
gtest_add_tests(TARGET HemiolaTest)
set_target_properties(HemiolaTest
    PROPERTIES
    CXX_STANDARD 17
    )
