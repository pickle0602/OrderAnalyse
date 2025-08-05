cmake_minimum_required(VERSION 3.5)
project(OrderAnalyse)

include_directories(include)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(SRCS
  src/main.cpp
)


find_package(GTest REQUIRED)
find_package(Threads REQUIRED)

add_executable(${PROJECT_NAME} ${SRCS})
add_executable(run_tests src/test.cpp)

target_compile_options(run_tests PRIVATE -pthread -fprofile-arcs -ftest-coverage -O0)
target_link_libraries(run_tests PRIVATE gcov Threads::Threads)
target_link_libraries(run_tests PRIVATE GTest::GTest GTest::Main)

enable_testing()
add_test(NAME CompletelyTest COMMAND run_tests)
