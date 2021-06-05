cmake_minimum_required (VERSION 2.8)
project (neblina-core)

include(CTest)

enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIR}) 

find_package(OpenCL REQUIRED)
include_directories( ${OPENCL_INCLUDE_DIR} )

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
#set(API_LIBRARIES ${OPENCL_LIBRARIES})
#set(API_INCLUDE_DIRS ${OPENCL_INCLUDE_DIRS})

add_compile_options(
  "-Wall" "-Wpedantic" "-Wextra" "-fexceptions"
  "-O0;-g3;-ggdb"
)
message("OpenCL_LIBRARY ${OpenCL_LIBRARY}")
message("PROJECT ${PROJECT_SOURCE_DIR}")
set(HEADER_PATH ${PROJECT_SOURCE_DIR}/include)
set(SOURCE_PATH ${PROJECT_SOURCE_DIR}/src)
set(DIST_PATH ${PROJECT_SOURCE_DIR}/dist)
set(DIST_INCLUDE_PATH ${PROJECT_SOURCE_DIR}/dist/include)
set(PROJECT_TEST_DIR ${PROJECT_SOURCE_DIR}/test)

include_directories("${HEADER_PATH}")
include_directories("..")
LINK_DIRECTORIES(${PROJECT_DIR})

add_library(neblina-core SHARED ${SOURCE_PATH}/libneblina.c ${SOURCE_PATH}/clutils.c ${SOURCE_PATH}/neblina_list.c ${SOURCE_PATH}/neblina_std.c )
target_link_libraries(neblina-core PRIVATE Threads::Threads ${OpenCL_LIBRARY})

add_executable(neblina-core_test ${PROJECT_TEST_DIR}/neblina-core_test.cpp)
target_link_libraries(neblina-core_test PRIVATE Threads::Threads ${OpenCL_LIBRARY} ${GTEST_BOTH_LIBRARIES} neblina-core)

add_test( neblina-core_test neblina-core_test )


if (EXISTS ${PROJECT_BINARY_DIR}/libneblina-core.so)
  file(COPY ${PROJECT_BINARY_DIR}/libneblina-core.so DESTINATION ${PROJECT_TEST_DIR} )
  #file(RENAME ${PROJECT_TEST_DIR}/libneblina-core.so ${PROJECT_TEST_DIR}/neblina-core.so)
endif()

# Installs the library
include(GNUInstallDirs)

install (TARGETS neblina-core DESTINATION ${CMAKE_INSTALL_LIBDIR})
install (FILES ${HEADER_PATH}/neblina.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install (FILES ${HEADER_PATH}/neblina_list.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install (FILES ${HEADER_PATH}/neblina_std.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install (FILES ${HEADER_PATH}/libneblina.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install (FILES ${HEADER_PATH}/clutils.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install (FILES ${HEADER_PATH}/vector.cl DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})