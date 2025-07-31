cmake_minimum_required(VERSION 3.5)
project(OrderAnalyse)

include_directories(include)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 全局优化标志
if(MSVC)
    # MSVC编译器优化
    add_compile_options(/O2 /Ob2 /Oi /Ot /Oy /GL)
    add_link_options(/LTCG)
else()
    # GCC/Clang编译器优化
    add_compile_options(-O3 -march=native -ffast-math -flto -fno-exceptions -fno-rtti)
    add_link_options(-flto)
endif()


set(SRCS
  src/main.cpp
)

add_executable(${PROJECT_NAME} ${SRCS})
add_executable(test src/test.cpp)
