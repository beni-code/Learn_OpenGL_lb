#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

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
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // 一行前三个为坐标xyz，后两个为纹理坐标st
        0.0f, 0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.0f, 0.5f, 1.0f
    };

    Shader shaderProgram("shaders/vertex.glsl", "shaders/fragment.glsl");

    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // 纹理的ID
    GLuint texture;
    glGenTextures(1, &texture);
    // 绑定纹理
    glBindTexture(GL_TEXTURE_2D, texture);

    // S轴纹理环绕方式：重复
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    // T轴纹理环绕方式：重复
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);

    // 纹理缩小过滤方式：在两个邻近的mipmap之间使用线性插值，并使用线性插值进行采样
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
    // 纹理放大过滤方式：线性过滤
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    // 加载一个图片
    int texWidth, texHeight; // 图片的宽高
    int nrChannels; // 图片颜色通道的个数
    stbi_uc* texData = stbi_load("wall.jpg", &texWidth, &texHeight, &nrChannels, 0);

    if (texData)
    {
        // 使用前面载入的图片数据生成一个纹理
        /**
         * 参数1：绑定目标，（任何绑定到GL_TEXTURE_1D和GL_TEXTURE_3D的纹理不会受到影响）。
         * 参数2：为纹理指定多级渐远纹理的级别。如果你希望单独手动设置每个多级渐远纹理的级别的话。这里我们填0，也就是基本级别。
         * 参数3：第三个参数告诉OpenGL我们希望把纹理储存为何种格式。我们的图像只有RGB值，因此我们也把纹理储存为RGB值。
         * 参数4：参数5：设置最终的纹理的宽度和高度
         * 参数6：总是被设为0（历史遗留的问题）
         * 参数7：第七第八个参数定义了源图的格式和数据类型。我们使用RGB值加载这个图像，并把它们储存为char(byte)数组，我们将会传入对应值。
         * 参数8：最后一个参数是实际的图像数据。
         */
        glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, texWidth, texHeight, 0,GL_RGB,GL_UNSIGNED_BYTE, texData);
        /**
         * Mipmap生成
         * 当调用glTexImage2D时，当前绑定的纹理对象就会被附加上纹理图像。然而，目前只有基本级别(Base-level)的纹理图像被加载了，如果要使用多级渐远纹理，我们必须手动设置所有不同的图像（不断递增第二个参数）
         * 或者，直接在生成纹理之后调用glGenerateMipmap。这会为当前绑定的纹理自动生成所有需要的多级渐远纹理。
         */
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }

    stbi_image_free(texData);

    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,GL_STATIC_DRAW);

    glVertexAttribPointer(glGetAttribLocation(shaderProgram.ID, "aPosition"), 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                          (void*)0);
    // 启用顶点属性
    glEnableVertexAttribArray(glGetAttribLocation(shaderProgram.ID, "aPosition"));

    glVertexAttribPointer(glGetAttribLocation(shaderProgram.ID, "aTexCoord"), 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                          (void*)(3 * sizeof(float)));
    // 启用顶点属性
    glEnableVertexAttribArray(glGetAttribLocation(shaderProgram.ID, "aTexCoord"));

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        shaderProgram.use();
        glBindVertexArray(VAO);
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
