cmake_minimum_required(VERSION 3.5.0)
project(kmeans VERSION 0.1.0 LANGUAGES C CXX)

# Include directories
include_directories(include)

# Find OpenMP package
find_package(OpenMP REQUIRED)

# Add executable targets
add_executable(experiment main.cpp)

# Enable optimization flags for the compiler
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -march=native -ffast-math -funroll-loops -ftree-vectorize")

# Link OpenMP
target_link_libraries(experiment PRIVATE OpenMP::OpenMP_CXX)
