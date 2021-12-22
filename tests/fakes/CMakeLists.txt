add_library(fakes
    SHARED
    FakeDevice.cpp)

target_include_directories(fakes
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )

set_target_properties(fakes
    PROPERTIES
    CXX_STANDARD 17
    )
target_compile_options(fakes PRIVATE
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Werror>
    )

target_link_libraries(fakes hemiola)
