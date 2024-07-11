cmake_minimum_required(VERSION 3.0)

# Set the project name
project(CV6_project)

# Find OpenCV library
find_package(OpenCV REQUIRED)

# Include directories
include_directories(include ${OpenCV_INCLUDE_DIRS})

# Set the C++ standard
set(CMAKE_CXX_STANDARD 14)

# Source files
set(SOURCES
    src/Main.cpp
    src/VideoHelpers.cpp
    src/ObjectRecognition.cpp  # Include this if ObjectRecognition.cpp is part of your project
)

# Create executable
add_executable(main ${SOURCES})

# Link against OpenCV library
target_link_libraries(main ${OpenCV_LIBS})
