cmake_minimum_required (VERSION 2.8)
project (neblina-core)

include(CTest)

enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIR}) 

find_package(OpenCL REQUIRED)
set(API_LIBRARIES ${OPENCL_LIBRARIES})
set(API_INCLUDE_DIRS ${OPENCL_INCLUDE_DIRS})


message("PROJECT ${PROJECT_SOURCE_DIR}")
set(HEADER_PATH ${PROJECT_SOURCE_DIR}/include)
set(SOURCE_PATH ${PROJECT_SOURCE_DIR}/src)
set(DIST_PATH ${PROJECT_SOURCE_DIR}/dist)
set(DIST_INCLUDE_PATH ${PROJECT_SOURCE_DIR}/dist/include)
set(PROJECT_TEST_DIR ${PROJECT_SOURCE_DIR}/test)

include_directories("${HEADER_PATH}")

add_library(neblina-core SHARED ${SOURCE_PATH}/libneblina.c ${SOURCE_PATH}/clutils.c)

add_executable(neblina-core_test ${PROJECT_TEST_DIR}/neblina-core_test.cpp)
target_link_libraries(neblina-core_test ${GTEST_BOTH_LIBRARIES} neblina-core)

add_test( neblina-core_test neblina-core_test )


if (EXISTS ${PROJECT_BINARY_DIR}/libneblina-core.so)
  file(COPY ${PROJECT_BINARY_DIR}/libneblina-core.so DESTINATION ${PROJECT_TEST_DIR} )
  file(RENAME ${PROJECT_TEST_DIR}/libneblina-core.so ${PROJECT_TEST_DIR}/neblina-core.so)
endif()

install (TARGETS neblina-core DESTINATION ${DIST_PATH})
install (FILES ${HEADER_PATH}/neblina-core.h DESTINATION ${DIST_INCLUDE_PATH})
install (FILES ${DIST_PATH}/libneblina-core.so DESTINATION ${DIST_PATH} RENAME neblina-core.so)