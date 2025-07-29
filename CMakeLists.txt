cmake_minimum_required(VERSION 3.5)
project(OrderAnalyse)

include_directories(include)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SRCS
  src/main.cpp
)

add_executable(${PROJECT_NAME} ${SRCS})
