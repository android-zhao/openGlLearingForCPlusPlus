#cmake_minimum_required(VERSION 3.19)
#project(OpenGlLearn)
#
#set(CMAKE_CXX_STANDARD 14)
#
## 添加头文件
#set(GLEW_H /usr/local/Cellar/glew/2.2.0_1/include/GL)
#set(GLFW_H /usr/local/Cellar/glfw/3.3.4/include/GLFW)
#include_directories(${GLEW_H} ${GLFW_H})
#
#set(GLM /Users/didi/code/Clion/openGlTest/glm/)
#include_directories(${GLM} ${GLM})
#
#set(GLM_GTC /Users/didi/code/Clion/openGlTest/glm/gtc)
#include_directories(${GLM_GTC} ${GLM_GTC})
#
#set(GLM_DETAIL /Users/didi/code/Clion/openGlTest/glm/detail)
#include_directories(${GLM_DETAIL} ${GLM_DETAIL})
#
#set(GLM_EXT /Users/didi/code/Clion/openGlTest/glm/ext)
#include_directories(${GLM_EXT} ${GLM_EXT})
#
#set(GLM_GTX /Users/didi/code/Clion/openGlTest/glm/gtx)
#include_directories(${GLM_GTX} ${GLM_GTX})
#
#set(GLM_SIMD /Users/didi/code/Clion/openGlTest/glm/simd)
#include_directories(${GLM_SIMD} ${GLM_SIMD})
#
#
## 添加目标链接
#set(GLEW_LINK /usr/local/Cellar/glew/2.2.0_1/lib/libGLEW.2.2.dylib)
#set(GLFW_LINK /usr/local/Cellar/glfw/3.3.4/lib/libglfw.3.dylib)
##link_libraries(${OPENGL} ${GLEW_LINK} ${GLFW_LINK})
#link_libraries("-framework OpenGL" ${GLEW_LINK} ${GLFW_LINK})
#
## 执行编译命令
#set(SOURCE_FILES
#        ../src/main.cpp
#        )
##set(SOURCE_FILES
##        src/MainTest.cpp
##        )
##set(SOURCE_FILES
##        src/MainCompare.cpp
##        )
#
#add_executable(openGlTest ${SOURCE_FILES})
#
#
#if (APPLE)
#    target_link_libraries(openGlTest "-framework OpenGL")
#    target_link_libraries(openGlTest "-framework GLUT")
#endif()
