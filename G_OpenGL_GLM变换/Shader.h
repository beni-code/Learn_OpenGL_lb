#pragma once
#include <iostream>
#include <string>
#include <glad/glad.h>
#include <fstream>
#include <sstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
public:
    GLuint ID;
    Shader(const char* vertexPath, const char* fragmentPath);
    void use();

    // uniform
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;

    /**
     * Matrix4fv后缀的glUniform函数把矩阵数据发送给着色器
     * 第二个参数告诉OpenGL我们将要发送多少个矩阵
     * 第三个参数询问我们是否希望对我们的矩阵进行转置(Transpose)，也就是说交换我们矩阵的行和列。OpenGL开发者通常使用一种内部矩阵布局，叫做列主序(Column-major Ordering)布局。
     *         GLM的默认布局就是列主序，所以并不需要转置矩阵
     * 最后一个参数是真正的矩阵数据。要先用GLM的自带的函数value_ptr来变换这些数据
     */
    void setTransform(const std::string& name, glm::mat4 value) const;
};
