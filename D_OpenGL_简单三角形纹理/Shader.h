#pragma once
#include <iostream>
#include <string>
#include <glad/glad.h>
#include <fstream>
#include <sstream>

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
};
