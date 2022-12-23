#include "Shader.h"

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
    std::string vertexCode, fragmentCode;
    std::ifstream vShaderFile, fShaderFile;

    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    // 将两个着色器的代码从文件中读取出来
    try
    {
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStringStream, fShaderStringStream;
        vShaderStringStream << vShaderFile.rdbuf();
        fShaderStringStream << fShaderFile.rdbuf();
        vShaderFile.close();
        fShaderFile.close();
        vertexCode = vShaderStringStream.str();
        fragmentCode = fShaderStringStream.str();
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
    }

    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    GLuint vertex, fragment;
    int success;
    char infoLog[1024];

    // 编译顶点着色器
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode,NULL);
    glCompileShader(vertex);
    // 检查顶点着色器编译是否错误
    glGetShaderiv(vertex,GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertex, 1024,NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // 编译片段着色器
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode,NULL);
    glCompileShader(fragment);
    // 检查片段着色器编译是否错误
    glGetShaderiv(fragment,GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragment, 1024,NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // 创建着色器程序并链接着色器
    ID = glCreateProgram();
    glAttachShader(ID,vertex);
    glAttachShader(ID,fragment);
    glLinkProgram(ID);
    // 检查链接错误
    glGetProgramiv(ID,GL_LINK_STATUS,&success);
    if (!success)
    {
        glGetProgramInfoLog(ID,1024,NULL,infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    // 链接完可以删除两种着色器对象
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::use()
{
    glUseProgram(ID);
}

void Shader::setBool(const std::string& name, bool value) const
{
    glUniform1i(glGetUniformLocation(ID,name.c_str()),(int)value);
}

void Shader::setInt(const std::string& name, int value) const
{
    glUniform1i(glGetUniformLocation(ID,name.c_str()),value);
}

void Shader::setFloat(const std::string& name, float value) const
{
    glUniform1f(glGetUniformLocation(ID,name.c_str()),value);
}

void Shader::setTransform(const std::string& name, glm::mat4 value) const
{
    glUniformMatrix4fv(glGetUniformLocation(ID,name.c_str()),1,GL_FALSE,glm::value_ptr(value));
}
