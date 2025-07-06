cmake_minimum_required(VERSION 3.16)
project(screenarschlock)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# Find SDL3 and SDL3_ttf
find_package(SDL3 REQUIRED)
find_package(SDL3_ttf REQUIRED)

# Add executable
add_executable(screenarschlock main.cpp)

# Link SDL3 and SDL3_ttf
target_link_libraries(screenarschlock PRIVATE SDL3::SDL3 SDL3_ttf::SDL3_ttf)





