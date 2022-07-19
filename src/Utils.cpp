//
// Created by zhf on 2022/7/18.
//

#include "Utils.h"

#include "iostream"
#include "../include/GLFW/glfw3.h"
#include "../include/glad/glad.h"

#include <string>
#include <fstream>
#include <sstream>

void GlClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

bool GlLogCall(const char *fun, const char *file, int line) {
    while (GLenum error = glGetError()){
        cout<<"OpenGl Error:" <<error<<endl;
        cout<<"file:"<<file<<",LINE:"<<line<<",FUNC："<<fun <<endl;
        return false;
    }
    return true;
}


bool Utils::createAndCompileShader(char *glslStr, int glslType, int &shaderId) {
    unsigned  int shaderType = -1;
    if(glslType == TYPE_VERTERX ){
        shaderType =GL_VERTEX_SHADER;
    } else if(glslType == TYPE_FRAGMENT){
        shaderType = GL_FRAGMENT_SHADER;
    } else{
        cout << "please input right glslType\n" <<endl;
        return false;
    }

    GLCall(shaderId = glCreateShader(shaderType));
    GLCall( glShaderSource(shaderId, 1, &glslStr, NULL));
    GLCall( glCompileShader(shaderId));
    int success;
    char infoLog[512];
    GLCall( glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success));
    cout << "createAndCompileShader success is   " << success<< std::endl;
    if (!success)
    {
        GLCall( glGetShaderInfoLog(shaderId, 512, NULL, infoLog));
        if(glslType == TYPE_VERTERX){
            cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
            return false;
        } else{
            cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
            return false;
        }
    }
    cout << "success::SHADER::VERTEX::COMPILATION " <<  std::endl;
    return true;
}


int Utils::linkProgram(int &vertexId, int &fragmentId) {

    //shaderProgram为opengl运行时 GPU上跑的程序Id， 此程序id 会被用来查找顶点和片元的着色器中定义的变量
    unsigned int shaderProgram;
    GLCall( shaderProgram = glCreateProgram());
    GLCall( glAttachShader(shaderProgram, vertexId));
    GLCall( glAttachShader(shaderProgram, fragmentId));
    GLCall( glLinkProgram(shaderProgram));

    // check for linking errors
    int success = -1;
    string infoLog = "";
    GLCall( glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success));
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, const_cast<char *>(infoLog.c_str()));
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    std::cout << "linkProgram success result" << shaderProgram << std::endl;
    glDeleteShader(vertexId);
    glDeleteShader(fragmentId);
    return shaderProgram;


}

string Utils::readShaderFromFile(char *vertexPath) {
    std::string vertexCode = "";
    std::ifstream vShaderFile;
//    std::ifstream fShaderFile;
    vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);

    try
    {
        // open files
        vShaderFile.open(vertexPath);
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        // close file handlers
        vShaderFile.close();
        // convert stream into string
        vertexCode   = vShaderStream.str();
    }
    catch (std::ifstream::failure& e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: " << e.what() << std::endl;
    }
    return vertexCode;

}
