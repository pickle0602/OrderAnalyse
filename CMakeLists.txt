cmake_minimum_required(VERSION 3.5)
project(OrderAnalyse)

include_directories(include)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -fprofile-arcs -ftest-coverage -O0 -lgcov ")

set(SRCS
  src/main.cpp
)

add_executable(${PROJECT_NAME} ${SRCS})
add_executable(test src/test.cpp)
