//
// Created by zhf on 2022/7/18.
//


#include "iostream"
//#include "../include/glad/glad.h"
//#include "../include/GLFW/glfw3.h"
#include "Utils.h"

using namespace std;

void createOpenglElement(unsigned  int * vaoId,unsigned int * vboId,unsigned int *iboId ,
                         float *vertex, int vertexLength, int * index, int indexLength);

//void initVertexConfiguration(float * vertexArray,int vertexArrayLength, int * index,int indexLength);


static unsigned  int VAO = 0;
static unsigned  int VBO = 0;
static unsigned  int EBO = 0;

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
    GLFWwindow* window = glfwCreateWindow(1280, 720, "hello opengl", NULL, NULL);
//    GLFWwindow* window = glfwCreateWindow(580, 654, "hello opengl", NULL, NULL);

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
//    float vertices[] = {
//            0.5f,  0.5f, 0.0f,  /* top right    //0  */ 1.0f, 1.0f, // top right
//            0.5f, -0.5f, 0.0f,  /* bottom right //1  */ 1.0f, 0.0f, // bottom right
//            -0.5f, -0.5f, 0.0f,  /* bottom left //2  */ 0.0f, 0.0f, // bottom left
//            -0.5f,  0.5f, 0.0f ,  /* top left    //3  */ 0.0f, 1.0f  // top left
//    };

    float vertices[] = {
            1.0f,  1.0f, 0.0f,  /* top right    //0  */ 1.0f, 1.0f, // top right
            1.0f, -1.0f, 0.0f,  /* bottom right //1  */ 1.0f, 0.0f, // bottom right
            -1.0f, -1.0f, 0.0f,  /* bottom left //2  */ 0.0f, 0.0f, // bottom left
            -1.0f,  1.0f, 0.0f ,  /* top left    //3  */ 0.0f, 1.0f  // top left
    };
    //顶点坐标对应的索引坐标
    int indices[] = {
            // note that we start from 0!
            0, 1, 3,  // first Triangle
            1, 2, 3   // second Triangle
    };

    unsigned  int vaoId = 99;
    unsigned int vboId = 99;
    unsigned int eboId = 99;

    createOpenglElement(&vaoId,&vboId,&eboId,vertices, (sizeof (vertices) /sizeof (float )) ,
                        indices, (sizeof (indices) / sizeof (int )));
    //生成纹理
    string img_path = "D:\\code\\clion_workspace\\openGlLearingForCPlusPlus\\res\\tescat2.jpg";
    unsigned  int textureId =  utils.createTexture(img_path);

    //获取片元着色器中采样器的id并指令运行时纹理插槽位置
    const string texName = "textureName";
    int textLocation =  utils.getUninformId(renderId,texName.c_str());
    utils.setUniform1i(textLocation,0);

    cout<< "textureId:" << textureId << endl;
    cout<<"vaoId: " <<vaoId<<endl;
    cout<<"vboId: " <<vboId<<endl;
    cout<<"iboId: " <<eboId<<endl;


    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        //初始化背景颜色
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        //激活0号纹理插槽
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureId);

        // draw our first triangle
        GLCall(glUseProgram(renderId));

        GLCall(glBindVertexArray(vaoId)); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));
        // glBindVertexArray(0); // no need to unbind it every time

        // -------------------------------------------------------------------------------
        GLCall(glfwSwapBuffers(window));
        GLCall(glfwPollEvents());
    }

    //reinterpret_cast 强制转换
    //删除
    glDeleteVertexArrays(1,&vaoId);
    glDeleteBuffers(1, &vboId);
    glDeleteBuffers(1, &eboId);
    glDeleteProgram(renderId);
    /*****************标准opengl 代码 在不同平台上 使用opengl 均需要以上代码*********************/

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}


//注意传入的是指针，需要在opengl中使用指针和解指针

void createOpenglElement(unsigned  int * vaoId,unsigned int * vboId,unsigned int *iboId ,
                         float *vertex, int vertexLength, int * index, int indexLength)
{
    cout << "createOpenglElement :" << "vertexLength:" <<vertexLength << ",indexLength :" <<indexLength<<endl;
    std::cout << "createOpenglElement   指针长度 sizeof(vertexArray) : " <<  sizeof(vertex) << std::endl;
    std::cout << "createOpenglElement   指针长度 sizeof(index ): " <<  sizeof(index) << std::endl;


    //创建和绑定vao
    GLCall( glGenVertexArrays(1, vaoId));
    GLCall( glBindVertexArray( *vaoId));

    //创建和绑定vbo，并为VBO指定数据源长度
    GLCall( glGenBuffers(1, vboId));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, *vboId));
    //glBufferData 函数第二个参数 指的是 顶点坐标的长度，一般是定义的顶点个数 * 每一个定的类型长度(sizeof (类型) )，顶点坐标类型一般是float
    GLCall(glBufferData(GL_ARRAY_BUFFER,vertexLength  *sizeof(float) , (void *)vertex, GL_STATIC_DRAW));

    //创建和绑定ibo并为IBO 指定具体数据源和长度
    GLCall( glGenBuffers(1, iboId));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *iboId));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexLength *sizeof(float), index, GL_STATIC_DRAW));


    //顶点坐标中包含了纹理 和 顶点坐标的

    //关键步骤，指定传递出去的数据之间的排列方式和数据大小 数据类型等
    GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, 5 * sizeof(GLfloat), (void*)0));
    GLCall(glEnableVertexAttribArray(0));

    //关键步骤，指定传递高gpu的顶点坐标数据中的纹理的位置，stride 等
    GLCall(glVertexAttribPointer(1, 2, GL_FLOAT, GL_TRUE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(float))));
    GLCall(glEnableVertexAttribArray(1));

    //解绑
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
