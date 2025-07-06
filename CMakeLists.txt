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

# Absolute path to the rsc directory
get_filename_component(RSC_PATH "${CMAKE_CURRENT_SOURCE_DIR}/rsc" ABSOLUTE)

# Pass it as a macro definition to the compiler
target_compile_definitions(screenarschlock PRIVATE RSC_PATH="${RSC_PATH}")

# Link SDL3 and SDL3_ttf
target_link_libraries(screenarschlock PRIVATE SDL3::SDL3 SDL3_ttf::SDL3_ttf)

# install under /usr/local/bin i think...
install(TARGETS screenarschlock DESTINATION bin)



