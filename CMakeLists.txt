cmake_minimum_required(VERSION 3.5)
project(OrderAnalyse)

include_directories(include)

set(SRCS
  src/main.cpp
)
add_executable(${PROJECT_NAME} ${SRCS})
