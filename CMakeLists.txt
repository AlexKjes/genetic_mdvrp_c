cmake_minimum_required(VERSION 3.8)
project(assign1_c)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS -O3)

link_libraries(gsl openblas m)

set(SOURCE_FILES main.c mdvrp.c mdvrp.h genetics.c genetics.h util.c util.h output.c output.h)
add_executable(assign1_c ${SOURCE_FILES})