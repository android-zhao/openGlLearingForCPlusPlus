//
// Created by zhf on 2022/7/18.
//

#ifndef OPENGLLEARN_UTILS_H
#define OPENGLLEARN_UTILS_H

#include "iostream"
#include "../include/GLFW/glfw3.h"
#include "../include/glad/glad.h"




using namespace std;

void GlClearError();

bool GlLogCall(const char* fun,const char* file,int  line);


#define ASSERT(x) if(!(x) ) cout<<"file:"<<__FILE__<<",LINE:"<<__LINE__<<",FUNC："<<__FUNCTION__ <<endl;

//检测错误的宏定义
# define GLCall(x) GlClearError(); \
x;                            \
ASSERT(GlLogCall(#x,__FILE__,__LINE__))



class Utils {
public:
    static  const int TYPE_VERTERX = 0;
    static  const int TYPE_FRAGMENT = 1;

    string readShaderFromFile(char * glslStr);
    bool createAndCompileShader(char* shaderStr,int glsltype,int &shaderId);
    int linkProgram(int &vertexId,int &fragmentId);

};


#endif //OPENGLLEARN_UTILS_H
