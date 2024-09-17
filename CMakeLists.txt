cmake_minimum_required (VERSION 2.8)
project (neblina-core)

include(CTest)

enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIR}) 

#find_package(OpenCL REQUIRED)
#include_directories( ${OPENCL_INCLUDE_DIR} )

# Set the C standard to C99
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
# set(CMAKE_C_EXTENSIONS OFF)  # Optional: Disable compiler-specific extensions


set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()
#set(API_LIBRARIES ${OPENCL_LIBRARIES})
#set(API_INCLUDE_DIRS ${OPENCL_INCLUDE_DIRS})

add_compile_options(
  "-Wall" "-Wpedantic" "-Wextra" "-fexceptions"
  "-O3;-g3;-ggdb"
)
#message("OpenCL_LIBRARY ${OpenCL_LIBRARY}")
message("PROJECT ${PROJECT_SOURCE_DIR}")
set(HEADER_PATH ${PROJECT_SOURCE_DIR}/include)
set(SOURCE_PATH ${PROJECT_SOURCE_DIR}/src)
set(DIST_PATH ${PROJECT_SOURCE_DIR}/dist)
set(DIST_INCLUDE_PATH ${PROJECT_SOURCE_DIR}/dist/include)
set(PROJECT_TEST_DIR ${PROJECT_SOURCE_DIR}/test)
set(LCOV_PATH /usr/bin/lcov)
set(GENHTML_PATH /usr/bin/genhtml)

include_directories("${HEADER_PATH}")
include_directories("..")
LINK_DIRECTORIES(${PROJECT_DIR})

if(CODE_COVERAGE)
   if(CMAKE_COMPILER_IS_GNUCXX)
       message("Building with lcov Code Coverage Tools")
       # Warning/Error messages
       if(NOT (CMAKE_BUILD_TYPE STREQUAL "Debug"))
           message(WARNING "Code coverage results with an optimized (non-Debug) build may be misleading")
       endif()
       if(NOT LCOV_PATH)
           message(FATAL_ERROR "lcov not found! Aborting...")
       endif()
       if(NOT GENHTML_PATH)
           message(FATAL_ERROR "genhtml not found! Aborting...")
       endif()
       set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --coverage -fprofile-arcs -ftest-coverage")
       set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage -fprofile-arcs -ftest-coverage")
   else()
       message(FATAL_ERROR "Code coverage requires Clang or GCC. Aborting.")
   endif()
endif()

add_library(neblina-core SHARED ${SOURCE_PATH}/libneblina.c 
                                ${SOURCE_PATH}/neblina_std.c 
                                 )

add_library(neblina-cpu-bridge SHARED ${SOURCE_PATH}/libneblina-cpu-bridge-vector.c
                                      ${SOURCE_PATH}/libneblina-cpu-bridge-list.c
                                      ${SOURCE_PATH}/libneblina-cpu-bridge-matrix.c
                                      ${SOURCE_PATH}/libneblina-cpu-bridge-smatrix.c
                                      ${SOURCE_PATH}/libneblina-cpu-bridge-complex.c
                                      ${SOURCE_PATH}/libneblina-cpu-bridge-api-impl.c
                                 )

target_link_libraries(neblina-core PRIVATE Threads::Threads ${CMAKE_DL_LIBS})

# Define the source and destination paths
set(source_folder "/usr/local/lib")
set(link_name "/usr/local/lib64")

# Create a custom command to create the symbolic link
add_custom_command(
    OUTPUT ${link_name}
    COMMAND ${CMAKE_COMMAND} -E create_symlink ${source_folder} ${link_name}
    DEPENDS ${source_folder}
)

# Create symbolic link
add_custom_target(create_symbolic_link DEPENDS ${link_name})	

# Create the coverage target
add_custom_target(coverage
    COMMAND lcov --directory . --capture --output-file coverage.info
    COMMAND lcov --remove coverage.info '/usr/*' --output-file coverage.info
    COMMAND lcov --list coverage.info
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Generating code coverage report"
)

# Create the coverage-html target
add_custom_target(coverage-html
    COMMAND genhtml coverage.info --output-directory coverage-html
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Generating HTML code coverage report"
)

#add_executable(vector_test_cpu_bridge ${PROJECT_TEST_DIR}/vector_test_cpu_bridge.cpp)
#target_link_libraries(vector_test_cpu_bridge PRIVATE Threads::Threads ${GTEST_BOTH_LIBRARIES} neblina-core)

add_executable(vector_test ${PROJECT_TEST_DIR}/vector_test.cpp)
target_link_libraries(vector_test PRIVATE Threads::Threads ${OpenCL_LIBRARY} ${GTEST_BOTH_LIBRARIES} neblina-core)

add_executable(matrix_test ${PROJECT_TEST_DIR}/matrix_test.cpp)
target_link_libraries(matrix_test PRIVATE Threads::Threads ${OpenCL_LIBRARY} ${GTEST_BOTH_LIBRARIES} neblina-core)

add_executable(sparse_matrix_test ${PROJECT_TEST_DIR}/sparse_matrix_test.cpp)
target_link_libraries(sparse_matrix_test PRIVATE Threads::Threads ${OpenCL_LIBRARY} ${GTEST_BOTH_LIBRARIES} neblina-core)

add_executable(neblina_test ${PROJECT_TEST_DIR}/neblina_test.cpp)
target_link_libraries(neblina_test PRIVATE Threads::Threads ${OpenCL_LIBRARY} ${GTEST_BOTH_LIBRARIES} neblina-core)

add_test( vector_test vector_test )
add_test( matrix_test matrix_test )
add_test( sparse_matrix_test sparse_matrix_test )
add_test( neblina_test neblina_test )


if (EXISTS ${PROJECT_BINARY_DIR}/libneblina-core.so)
  file(COPY ${PROJECT_BINARY_DIR}/libneblina-core.so DESTINATION ${PROJECT_TEST_DIR} )
  #file(RENAME ${PROJECT_TEST_DIR}/libneblina-core.so ${PROJECT_TEST_DIR}/neblina-core.so)
endif()

# Installs the library
include(GNUInstallDirs)

install (TARGETS neblina-core DESTINATION ${CMAKE_INSTALL_LIBDIR})
install (TARGETS neblina-cpu-bridge DESTINATION ${CMAKE_INSTALL_LIBDIR})
install (FILES ${HEADER_PATH}/neblina.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install (FILES ${HEADER_PATH}/neblina_list.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install (FILES ${HEADER_PATH}/neblina_std.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install (FILES ${HEADER_PATH}/neblina_vector.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install (FILES ${HEADER_PATH}/neblina_matrix.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install (FILES ${HEADER_PATH}/neblina_smatrix.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install (FILES ${HEADER_PATH}/neblina_complex.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install (FILES ${HEADER_PATH}/neblina_list.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install (FILES ${HEADER_PATH}/libneblina.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install (FILES ${HEADER_PATH}/bridge_api.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
#install (FILES ${HEADER_PATH}/clutils.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
#install (FILES ${HEADER_PATH}/vector.cl DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})