cmake_minimum_required(VERSION 3.8)
project(assign1_c)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES main.c mdvrp.c mdvrp.h genetics.c genetics.h)
add_executable(assign1_c ${SOURCE_FILES})