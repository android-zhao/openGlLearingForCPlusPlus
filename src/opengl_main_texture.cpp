//
// Created by zhf on 2022/7/18.
//


#include "iostream"
//#include "../include/glad/glad.h"
//#include "../include/GLFW/glfw3.h"
#include "Utils.h"

using namespace std;

void createOpenglElement(int &vaoId,int &vboId,int &iboId,float *vertex,int vertexLength,int * index,int indexLength);


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

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))

    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    //设置opengl窗口显示的原点和宽高，opengl的显示视图窗口和 我们上面创建的 window窗口一样大小
    glViewport(0,0,960,540);

    //设置刷新频率
    glfwSwapInterval(1);


    Utils utils;
    string verTextShaderPath = "D:\\code\\clion_workspace\\openGlLearingForCPlusPlus\\glsl\\cpp_vertex.glsl";
    string fragmentPath = "D:\\code\\clion_workspace\\openGlLearingForCPlusPlus\\glsl\\cpp_fragment.glsl";
    string  vertexStr = utils.readShaderFromFile(const_cast<char *>(verTextShaderPath.c_str()));
    string  fragmentStr = utils.readShaderFromFile(const_cast<char *>(fragmentPath.c_str()));

    int vertexId = -1;
    int fragmentId = -1;
    utils.createAndCompileShader(const_cast<char *>(vertexStr.c_str()), utils.TYPE_VERTERX, vertexId);
    utils.createAndCompileShader(const_cast<char *>(fragmentStr.c_str()), utils.TYPE_FRAGMENT, fragmentId);

    cout<<"vertexId:" <<vertexId<<endl;
    cout<<"fragmentId:" <<fragmentId<<endl;

   int renderId =  utils.linkProgram(vertexId,fragmentId);

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

    int vaoId = -1;
    int vboId = -1;
    int iboId = -1;
    createOpenglElement(vaoId, vboId, iboId, vertices, (sizeof (vertices)/sizeof (float)) ,
                        indices, (sizeof (indices) / sizeof (int )));

    cout<<"vaoId: " <<vaoId<<endl;
    cout<<"vboId: " <<vboId<<endl;
    cout<<"iboId: " <<iboId<<endl;

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        //初始化背景颜色
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw our first triangle
        GLCall(glUseProgram(renderId));

        GLCall(glBindVertexArray(vaoId)); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));
        // glBindVertexArray(0); // no need to unbind it every time

        // -------------------------------------------------------------------------------
        GLCall(glfwSwapBuffers(window));
        GLCall(glfwPollEvents());
    }

    //删除
    glDeleteVertexArrays(1, reinterpret_cast<const GLuint *>(&vaoId));
    glDeleteBuffers(1, reinterpret_cast<const GLuint *>(&vboId));
    glDeleteBuffers(1, reinterpret_cast<const GLuint *>(&iboId));
    glDeleteProgram(renderId);
    /*****************标准opengl 代码 在不同平台上 使用opengl 均需要以上代码*********************/

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

void createOpenglElement(int &vaoId,int &vboId,int &eboId,float *vertex,int vertexLength,int * index,int indexLength){
    GLCall( glGenVertexArrays(1, reinterpret_cast<GLuint *>(vaoId)));
    GLCall( glGenBuffers(1, reinterpret_cast<GLuint *>(vboId)));
    GLCall( glGenBuffers(1, reinterpret_cast<GLuint *>(eboId)));

    //绑定VAO VBO 和EBO
    GLCall( glBindVertexArray(vaoId));
    std::cout << "initVertexConfiguration   sizeof(vertexArray) : " <<  sizeof(vertex) << std::endl;
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, vboId));
//    GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(vertexArray) * sizeof(float), (void *)vertexArray, GL_STATIC_DRAW));
//glBufferData 函数第二个参数 指的是 顶点坐标的长度
    GLCall(glBufferData(GL_ARRAY_BUFFER, vertexLength, (void *)vertex, GL_STATIC_DRAW));

    std::cout << "initVertexConfiguration   sizeof(index : " <<  sizeof(index) << std::endl;
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboId));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexLength, index, GL_STATIC_DRAW));

    //关键步骤，指定传递出去的数据之间的排列方式和数据大小 数据类型等
    GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0));
    GLCall(glEnableVertexAttribArray(0));

    //解绑
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
