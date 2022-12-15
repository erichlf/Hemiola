################## git our external dependencies ##################
include(ExternalProject)
find_package(Git REQUIRED)

set(EXTERNAL_INSTALL_LOCATION ${CMAKE_BINARY_DIR}/external)

include(cmake/fmt.cmake)
include(cmake/spdlog.cmake)
include(cmake/yaml-cpp.cmake)
