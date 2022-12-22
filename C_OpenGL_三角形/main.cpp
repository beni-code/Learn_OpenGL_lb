#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "Shader.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window,GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwTerminate();
    }
}

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "gl1",NULL,NULL);
    if (window == NULL)
    {
        std::cout << "GLFW初始化失败" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "glad初始化失败" << std::endl;
        return -1;
    }

    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f,
        0.5f, -0.5f, 0.0f
    };

    Shader shaderProgram("shaders/vertex.glsl", "shaders/fragment.glsl");

    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    /**
     * glBufferData
     * 描述：  a function specifically targeted to copy user-defined data into the currently bound buffer
     *        该函数用于将数据复制进当前的缓冲区中
     * 参数1： the type of the buffer we want to copy data into: 我们想把数据复制进哪个类型的缓冲
     * 参数2： specifies the size of the data (in bytes) we want to pass to the buffer 我们想把多大的数据（单位字节）传给缓冲
     * 参数3： 发送的数据
     * 参数4： specifies how we want the graphics card to manage the given data. 我们希望显卡如何管理数据
     *          ● GL_STREAM_DRAW: the data is set only once and used by the GPU at most a few times. 设置一次数据，很少使用
     *          ● GL_STATIC_DRAW: the data is set only once and used many times. 设置一次，频繁使用
     *          ● GL_DYNAMIC_DRAW: the data is changed a lot and used many times. 数据经常变化且频繁使用
     */
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,GL_STATIC_DRAW);

    /**
     * glVertexAttribPointer
     * 描述：  tell OpenGL how it should interpret（解释） the vertex data
     * 参数1： specifies which vertex attribute we want to configure. 我们想要配置哪个顶点属性
     * 参数2： specifies the size of the vertex attribute. 顶点属性的大小，这里每次传一个3维坐标，一次有3个浮点数
     * 参数3： specifies the type of the data
     * 参数4： specifies if we want the data to be normalized. If we're inputting integer data types (int, byte) and we've set this to GL_TRUE, the integer data is normalized to 0 (or -1 for signed data) and 1 when converted to float. This is not relevant for us so we'll leave this at GL_FALSE.
     *          是否标准化数据
     * 参数5： 第五个参数叫做步长(Stride)，它告诉我们在连续的顶点属性组之间的间隔。由于下个组位置数据在3个float之后，我们把步长设置为3 * sizeof(float)。要注意的是由于我们知道这个数组是紧密排列的（在两个顶点属性之间没有空隙）我们也可以设置为0来让OpenGL决定具体步长是多少（只有当数值是紧密排列时才可用）。
     *        注: 这个参数的意思简单说就是从这个属性第二次出现的地方到整个数组0位置之间有多少字节
     *
     * 参数6： 最后一个参数的类型是void*，所以需要我们进行这个奇怪的强制类型转换。它表示位置数据在缓冲中起始位置的偏移量(Offset)。由于位置数据在数组的开头，所以这里是0。我们会在后面详细解释这个参数
     */
    glVertexAttribPointer(glGetAttribLocation(shaderProgram.ID, "aPosition"), 3,GL_FLOAT, false, 0, (void*)0);
    // 启用顶点属性
    glEnableVertexAttribArray(glGetAttribLocation(shaderProgram.ID, "aPosition"));

    // unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        shaderProgram.use();
        glBindVertexArray(VAO);

        /**
         * 第一个参数是我们打算绘制的OpenGL图元的类型
         * 第二个参数指定了顶点数组的起始索引
         * 最后一个参数指定我们打算绘制多少个顶点，这里是3
         */
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    // delete resource
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram.ID);

    glfwTerminate();
    return 0;
}
