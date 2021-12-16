find_package(GTest 1.8 REQUIRED)

add_executable(AnagramTest AnagramTest.cpp)
target_link_libraries(AnagramTest hemiola GTest::GTest GTest::Main -no-pie)
gtest_add_tests(TARGET AnagramTest)
