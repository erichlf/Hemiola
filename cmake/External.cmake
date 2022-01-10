################## git our external dependencies ##################
include(ExternalProject)
find_package(Git REQUIRED)

set(EXTERNAL_INSTALL_LOCATION ${CMAKE_BINARY_DIR}/external)

include(cmake/fmt.cmake)
include(cmake/nuspell.cmake)
include(cmake/spdlog.cmake)
