cmake_minimum_required(VERSION 3.1)

set(PROJECT_NAME StructureFromMotion)
project(${PROJECT_NAME})

add_executable(${PROJECT_NAME} main.cpp sfm_global.cpp sfm_global.hpp sfm.cpp sfm.hpp Tracker.cpp 
Tracker.hpp Mapper.cpp Mapper.hpp util.cpp util.hpp sfmgui.hpp sfmgui.cpp window.hpp window.cpp)

find_package(Eigen3 REQUIRED)
include_directories(${EIGEN3_INCLUDE_DIRS})

find_package(OpenCV REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC ${OpenCV_LIBS})
target_include_directories(${PROJECT_NAME} PUBLIC ${OpenCV_INCLUDE_DIRS})

find_package(OpenGL REQUIRED)

link_directories("C:/lib/OpenGL/GLFW/lib")
link_directories("C:/lib/OpenGL/GLEW/lib")


target_include_directories(${PROJECT_NAME} PUBLIC "C:/lib/OpenGL/GLEW/include")
target_include_directories(${PROJECT_NAME} PUBLIC "C:/lib/OpenGL/GLFW/include")
target_include_directories(${PROJECT_NAME} PUBLIC ${OPENGL_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} PUBLIC opengl32)

target_include_directories(${PROJECT_NAME} PUBLIC "C:/glm")
