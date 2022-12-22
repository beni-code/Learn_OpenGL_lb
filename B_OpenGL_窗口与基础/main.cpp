#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

/**
 * glad包含<gl.h>，所以务必放在第一位
 */


/**
 * 3D坐标转为2D坐标的处理过程是由OpenGL的图形渲染管线
 *  "./imgs/图形管线.png"
 *
 * 管线STEP前置：顶点数据输入
 *      注：不是所有的顶点都会渲染出来，标准化设备坐标(Normalized Device Coordinates)才会显示出来（各轴范围在-1.0到1.0的坐标）
 *          标准化设备坐标通过使用glViewport提供的宽高进行视口转换转换为屏幕空间坐标。然后将生成的屏幕空间坐标转换为片段，作为片段着色器的输入。
 *
 * 管线STEP① （必须）顶点着色器 3D坐标转换为另一种3D坐标，顶点基本处理
 *
 *     注：顶点着色器会在GPU上创建内存用于储存顶点数据
 *        我们通过顶点缓冲对象VBO管理这个内存，它会在__显存__中存储大量顶点，然后顶点着色器可以立即访问这些顶点
 *        unsigned int VBO; // VBO有一个唯一的ID
 *        glGenBuffers(1, &VBO);
 *
 *        声明完了之后绑定缓冲对象
 *        glBindBuffer(GL_ARRAY_BUFFER, VBO); // 将VBO只能绑定到GL_ARRAY_BUFFER上
 *        注：OpenGL可同时绑定多个不同类型的缓冲
 *
 *        这之后，我们使用的任何（在GL_ARRAY_BUFFER上的）缓冲调用都会用来配置当前绑定的缓冲(VBO)
 *        glBufferData函数会把顶点数据复制到缓冲的内存中
 *        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
 *        注：glBufferData是一个专门用来把用户定义的数据复制到当前绑定缓冲的函数
 *            第一个参数是目标缓冲的类型
 *            第二个参数指定传输数据的大小(以字节为单位)
 *            第三个参数是我们希望发送的实际数据
 *            第四个参数指定了我们希望显卡如何管理给定的数据
 *                  ● GL_STATIC_DRAW ：数据不会或几乎不会改变。
 *                  ● GL_DYNAMIC_DRAW：数据会被改变很多。
 *                  ● GL_STREAM_DRAW ：数据每次绘制时都会改变。  ------- 比如说一个缓冲中的数据将频繁被改变，那么使用的类型就是GL_DYNAMIC_DRAW或GL_STREAM_DRAW，这样就能确保显卡把数据放在能够高速写入的内存部分
 *     
 * 管线STEP② 图元装配 装配图元，一般是三角形，或顶点
 * 管线STEP③ （可选）几何着色器，输入图元形式的顶点，然乎通过产生新顶点的方式来__构造出新图元__从而生成其他形状
 * 管线STEP④ 光栅化：将图元映射为屏幕上相应的像素，并生成供片段着色器使用的片段。在片段着色器运行之前会进行<裁切 Clipping>，裁切会丢弃超出视图以外的所有像素，从而提升性能
 *    注：一个片段是渲染一个像素所需的所有数据
 * 管线STEP⑤ （必须）片段着色器计算像素的最终颜色，（通常，片段着色器包含3D场景的数据（比如光照、阴影、光的颜色等等），这些数据可以被用来计算最终像素的颜色。）
 * 管线STEP⑥ Alpha测试和混合阶段
 *    检测片段的对应的深度和模板值，从而判断像素在其他物体的前后，然后判断是否丢弃
 *    通过检查Alpha值（透明度）来对物体进行混合
 */

void framebuffer_size_callback(GLFWwindow* window,int width,int height)
{
    glViewport(0,0,width,height);
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

    glViewport(0,0,800,600);
    glfwSetFramebufferSizeCallback(window,framebuffer_size_callback);

    while (!glfwWindowShouldClose(window))
    {
        // 接受键盘输入
        processInput(window);

        // clearColor状态设置函数
        glClearColor(0.2f,0.3f,0.3f,1.0f);
        // 状态使用函数
        glClear(GL_COLOR_BUFFER_BIT);

        // checks if any events are triggered (like keyboard input or mouse movement events), updates the window state, and calls the corresponding functions (which we can register via callback methods). 
        glfwPollEvents();
        // swap the color buffer (a large 2D buffer that contains color values for each pixel in GLFW's window) that is used to render to during this render iteration and show it as output to the screen.
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}
