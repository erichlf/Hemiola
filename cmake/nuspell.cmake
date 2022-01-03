###### NUSPELL ######
set(NUSPELL_INSTALL_DIR ${EXTERNAL_INSTALL_LOCATION}/nuspell)
set(NUSPELL_INCLUDE_DIR ${NUSPELL_INSTALL_DIR}/include)
set(NUSPELL_LIB_DIR ${NUSPELL_INSTALL_DIR}/lib)
ExternalProject_Add(nuspell
  PREFIX ${NUSPELL_INSTALL_DIR}
  INSTALL_DIR ${NUSPELL_INSTALL_DIR}
  GIT_REPOSITORY https://github.com/nuspell/nuspell
  GIT_TAG "v5.0.1"
  CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=${NUSPELL_INSTALL_DIR}
  )

set(NUSPELL_FOUND TRUE)
set(NUSPELL_INCLUDE_DIRS ${NUSPELL_INSTALL_DIR}/include)
set(NUSPELL_LIBRARIES ${NUSPELL_INSTALL_DIR}/lib/libnuspell.so)
set(NUSPELL_LIBRARY_DIRS ${NUSPELL_INSTALL_DIR}/lib)
set(NUSPELL_EXTERNAL TRUE)

include_directories(${NUSPELL_INCLUDE_DIRS})
link_directories(${NUSPELL_LIBRARY_DIRS})

list(APPEND EXTERNAL_INCLUDES PUBLIC ${NUSPELL_INCLUDE_DIRS})
list(APPEND EXTERNAL_LIBS PUBLIC ${NUSPELL_LIBRARIES})

list(APPEND EXTERNAL_DEPENDENCIES nuspell)
