cmake_minimum_required(VERSION 3.10)
project(ZPG VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g ")
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(OpenGL_GL_PREFERENCE LEGACY)

add_compile_options(-Wall -Wextra -pedantic)

# Find required packages
find_package(glm REQUIRED)
find_package(OpenGL REQUIRED)
find_package(PkgConfig REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 3.3 REQUIRED)
find_package(assimp REQUIRED)

# Locate SOIL shared library
find_library(SOIL_LIBRARY SOIL PATHS /usr/lib64 /usr/lib)

if (NOT SOIL_LIBRARY)
    message(FATAL_ERROR "SOIL shared library not found. Ensure libSOIL.so is in /usr/lib64 or /usr/lib.")
endif()

# Include directory for SOIL headers
set(SOIL_INCLUDE_DIR "/usr/include/SOIL")  # Default for system-wide installation; adjust if necessary

if (NOT EXISTS ${SOIL_INCLUDE_DIR}/SOIL.h)
    message(FATAL_ERROR "SOIL header file not found in ${SOIL_INCLUDE_DIR}.")
endif()

include_directories(${GLEW_INCLUDE_DIRS} ${SOIL_INCLUDE_DIR})

# Find source files
file(GLOB_RECURSE SOURCES "src/*.cpp" "src/**/*.cpp")

# Define the executable
add_executable(zpg ${SOURCES})

# Link libraries
target_link_libraries(zpg PRIVATE
    glfw
    glm::glm
    GL
    ${OPENGL_gl_LIBRARY}
    ${GLEW_LIBRARIES}
    ${SOIL_LIBRARY}
    assimp
)
