###### YAML-CPP ######
set(YAML-CPP_INSTALL_DIR ${EXTERNAL_INSTALL_LOCATION}/yaml-cpp)
set(YAML-CPP_INCLUDE_DIR ${YAML-CPP_INSTALL_DIR}/include)
set(YAML-CPP_LIB_DIR ${YAML-CPP_INSTALL_DIR}/lib)

set(YAML-CPP_EXTRA_CXX_FLAGS "-fPIC")

ExternalProject_Add(yaml-cpp
  PREFIX ${YAML-CPP_INSTALL_DIR}
  INSTALL_DIR ${YAML-CPP_INSTALL_DIR}
  GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
  GIT_TAG "yaml-cpp-0.7.0"
  CMAKE_ARGS
  -DCMAKE_INSTALL_PREFIX:PATH=${YAML-CPP_INSTALL_DIR}
  -DCMAKE_CXX_FLAGS=${YAML-CPP_EXTRA_CXX_FLAGS}
  -DYAML_BUILD_SHARED_LIBS=OFF
  -DYAML_CPP_BUILD_TESTS=FALSE
)

set(YAML-CPP_FOUND TRUE)
set(YAML-CPP_INCLUDE_DIRS ${YAML-CPP_INSTALL_DIR}/include)
set(YAML-CPP_LIBRARIES ${YAML-CPP_INSTALL_DIR}/lib/libyaml-cpp.a)
set(YAML-CPP_LIBRARY_DIRS ${YAML-CPP_INSTALL_DIR}/lib)
set(YAML-CPP_EXTERNAL TRUE)

include_directories(${YAML-CPP_INCLUDE_DIRS})
link_directories(${YAML-CPP_LIBRARY_DIRS})

list(APPEND EXTERNAL_INCLUDES PUBLIC ${YAML-CPP_INCLUDE_DIRS})
list(APPEND EXTERNAL_LIBS PUBLIC ${YAML-CPP_LIBRARIES})

list(APPEND EXTERNAL_DEPENDENCIES yaml-cpp)
