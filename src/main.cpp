#include <iostream>

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


//顶点着色器脚本
const char *vertexShaderSource =
        "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "}\0";
//片元着色器脚本
const char *fragmentShaderSource =
        "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
        "}\n\0";

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void key_callback(GLFWwindow   * window, int key, int scancode, int action, int mode)
{
    //如果按下ESC，把windowShouldClose设置为True，外面的循环会关闭应用
    if(key==GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    std::cout<<"ESC"<<mode;
}
const int GLSL_TYPE_VERTEX = 0;
const int GLSL_TYPE_FRAGMENT = 1;

static unsigned  int VAO = 0;
static unsigned  int VBO = 0;
static unsigned  int EBO = 0;

bool createAndCompileShader(const char* glslStr,int glslType,int &shaderId )
{
    unsigned  int shaderType = -1;
    if(glslType == GLSL_TYPE_VERTEX ){
        shaderType =GL_VERTEX_SHADER;
    } else if(glslType == GLSL_TYPE_FRAGMENT){
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
        if(glslType == GLSL_TYPE_VERTEX){
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

int linkProgram(int  &vertexId,int &fragmentId )
{

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

void initVertexConfiguration(float * vertexArray,int vertexArrayLength, int * index,int indexLength){
    //创建顶点缓存数组 VAO 顶点数组VBO 和元素缓存数组（元素缓存）EBO
    GLCall( glGenVertexArrays(1, &VAO));
    GLCall( glGenBuffers(1, &VBO));
    GLCall( glGenBuffers(1, &EBO));

    //绑定VAO VBO 和EBO
    GLCall( glBindVertexArray(VAO));
    std::cout << "initVertexConfiguration   sizeof(vertexArray) : " <<  sizeof(vertexArray) << std::endl;
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, VBO));
//    GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(vertexArray) * sizeof(float), (void *)vertexArray, GL_STATIC_DRAW));
//glBufferData 函数第二个参数 指的是 顶点坐标的长度
    GLCall(glBufferData(GL_ARRAY_BUFFER, vertexArrayLength, (void *)vertexArray, GL_STATIC_DRAW));

    std::cout << "initVertexConfiguration   sizeof(index : " <<  sizeof(index) << std::endl;
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexLength, index, GL_STATIC_DRAW));

    //关键步骤，指定传递出去的数据之间的排列方式和数据大小 数据类型等
    GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0));
    GLCall(glEnableVertexAttribArray(0));

    //解绑
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

int main() {
    std::cout << "Hello, World!" << std::endl;

    if(!glfwInit())
        return -1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE ,GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif


//    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);//

    //创建窗口以及上下文,窗口大小为960 * 540，这个值可以随便更改，电脑会根据你配置的值生成对应大小的窗口显示器
    GLFWwindow* window = glfwCreateWindow(960, 540, "hello opengl", NULL, NULL);
    if(!window)
    {
        //创建失败会返回NULL
        glfwTerminate();
        std::cout << "create window failed" << std::endl;
        return -1;
    }
    glfwMakeContextCurrent (window);

    // Set the required callback functions
    glfwSetKeyCallback(window, key_callback);

    /********
     MAC系统采用的是glew 和 glfw 在PC上绘制 因此采用mac 操作系统时 需要采用glewInit 方法将opengl的程序起起来
    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    //  glewExperimental = GL_TRUE;

    //    int result = glewInit();
    //    cout << "result : "<<result<<endl;
    //    cout << "gl version "<<glGetString(GL_VERSION)<<endl;
     ************/

    /********
     * window 操作系统使用的是glad 和 glfw 来在 PC加载和使用opengl来显示，因此需要先使用 gladLoadGLLoader 来加载opengl程序
     */
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))

    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }


    /*****************标准opengl 代码 在不同平台上 使用opengl 均需要以下代码*********************/
    //设置opengl窗口显示的原点和宽高，opengl的显示视图窗口和 我们上面创建的 window窗口一样大小
    glViewport(0,0,960,540);

    //设置刷新频率
    glfwSwapInterval(1);

    //第一步，创建glsl脚本，即顶点着色器和片元着色器 并编译
    int vertexId  = -1;
    int fragmentId  = -1;
    cout << "first step "<<endl;
    createAndCompileShader(vertexShaderSource,GLSL_TYPE_VERTEX,vertexId);
    createAndCompileShader(fragmentShaderSource,GLSL_TYPE_FRAGMENT,fragmentId);
    cout << "first step end,vertexId"<<vertexId<<endl;
    cout << "first step end,fragmentId"<<fragmentId<<endl;

    cout << "Second step "<<endl;
    //第二步：生成使用顶点和片元着色器的opengl 程序 ID
    unsigned  int programId = linkProgram(vertexId,fragmentId);
    cout << "Second step result  programId is  "<< programId <<endl;

    //顶点坐标
    float vertices[] = {
            0.5f,  0.5f, 0.0f,  // top right    //0
            0.5f, -0.5f, 0.0f,  // bottom right //1
            -0.5f, -0.5f, 0.0f,  // bottom left //2
            -0.5f,  0.5f, 0.0f   // top left    //3
    };
    //顶点坐标对应的索引坐标
     int indices[] = {
             // note that we start from 0!
            0, 1, 3,  // first Triangle
            1, 2, 3   // second Triangle
    };
    //第三步：初始化顶坐标相关环境
    initVertexConfiguration(vertices,sizeof(vertices),indices,sizeof(indices));

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        //初始化背景颜色
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw our first triangle
        GLCall(glUseProgram(programId));

        GLCall(glBindVertexArray(VAO)); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));
        // glBindVertexArray(0); // no need to unbind it every time

        // -------------------------------------------------------------------------------
        GLCall(glfwSwapBuffers(window));
        GLCall(glfwPollEvents());
    }

    //删除
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(programId);
    /*****************标准opengl 代码 在不同平台上 使用opengl 均需要以上代码*********************/

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;

}
