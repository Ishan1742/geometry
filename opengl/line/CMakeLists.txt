cmake_minimum_required(VERSION 3.1...3.15)
project (line)

add_executable(line line.cpp)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS})

target_link_libraries(line ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})
