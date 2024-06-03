cmake_minimum_required(VERSION 3.0)

# Set the project name
project(main)

# Find OpenCV library
find_package(OpenCV REQUIRED)

include_directories(include ${OpenCV_INCLUDE_DIRS})

set(CMAKE_CXX_STANDARD 14)

# Create executable
add_executable(main src/main.cpp)

# Link against OpenCV library
target_link_libraries(main ${OpenCV_LIBS})
