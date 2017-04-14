cmake_minimum_required(VERSION 3.6)
project(CPPSparseLinearAlgebra)

set(CMAKE_CXX_STANDARD 11)

# Locate GTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

set(SOURCE_FILES main.cpp SparseVector.h)
add_executable(CPPSparseLinearAlgebra ${SOURCE_FILES})

set(SOURCE_FILES SparseVector.h SparseVectorTest.cpp)
add_executable(runTestsVector ${SOURCE_FILES})
target_link_libraries(runTestsVector ${GTEST_LIBRARIES} pthread)

set(SOURCE_FILES SparseVector.h SparseMatrix.h SparseMatrixTest.cpp)
add_executable(runTestsMatrix ${SOURCE_FILES})
target_link_libraries(runTestsMatrix ${GTEST_LIBRARIES} pthread)