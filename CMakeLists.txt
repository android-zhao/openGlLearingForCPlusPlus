cmake_minimum_required(VERSION 3.19)

project(OpenGlLearn)

set(CMAKE_CXX_STANDARD 14)

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/include)
link_directories(${PROJECT_SOURCE_DIR}/lib)

# 执行编译命令
set(SOURCE_FILES
        src/main.cpp
        src/glad.c
#        src/MainDemo.cpp
        )

#set(SOURCE_FILES
#        src/MainTest.cpp
#        )

add_executable(OpenGlLearn ${SOURCE_FILES})
#add_executable(OpenGlLearn src/MainDemo.cpp)




#方法一和方法二 取任何一个即可
#方法一：采用动态库 需要在cmake-build-debug 文件夹下面将glfw3.dll copy过去
#target_link_libraries(OpenGlLearn  -lopengl32  ${PROJECT_SOURCE_DIR}/lib/glfw3.dll)

#方法二：或者采用静态库 将 libglfw3.a 和 libglfw3dll.a 直接打包到程序中
target_link_libraries(OpenGlLearn  -lopengl32  ${PROJECT_SOURCE_DIR}/lib/libglfw3.a)
target_link_libraries(OpenGlLearn  -lopengl32 ${PROJECT_SOURCE_DIR}/lib/libglfw3dll.a)