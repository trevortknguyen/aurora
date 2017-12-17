#include <iostream>
#include <glad/glad.h>
#include <string>
#include <aurora/shader.hpp>

GLuint createShaderProgram(const char* vertexSource, const char* fragmentSource)
{
    GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShaderId, 1, &vertexSource, NULL);
    glCompileShader(vertexShaderId);
    checkShader(vertexShaderId, "vertex"); 

    GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShaderId, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShaderId);
    checkShader(fragmentShaderId, "fragment"); 

    // generate the shader program
    GLuint shaderProgramId = glCreateProgram();
    glAttachShader(shaderProgramId, vertexShaderId);
    glAttachShader(shaderProgramId, fragmentShaderId);
    glLinkProgram(shaderProgramId);

    // program is linked
    // individual shaders are not needed
    glDeleteShader(vertexShaderId);
    glDeleteShader(fragmentShaderId);

    return shaderProgramId;
}

GLuint createShaderProgram(const char* vertexSource, const char* geometrySource, const char* fragmentSource)
{
    GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShaderId, 1, &vertexSource, NULL);
    glCompileShader(vertexShaderId);
    checkShader(vertexShaderId, "vertex"); 

    GLuint geometryShaderId = glCreateShader(GL_GEOMETRY_SHADER);
    glShaderSource(geometryShaderId, 1, &geometrySource, NULL);
    glCompileShader(geometryShaderId);
    checkShader(geometryShaderId, "geometry"); 

    GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShaderId, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShaderId);
    checkShader(fragmentShaderId, "fragment"); 

    // generate the shader program
    GLuint shaderProgramId = glCreateProgram();
    glAttachShader(shaderProgramId, vertexShaderId);
    glAttachShader(shaderProgramId, geometryShaderId);
    glAttachShader(shaderProgramId, fragmentShaderId);
    glLinkProgram(shaderProgramId);

    // program is linked
    // individual shaders are not needed
    glDeleteShader(vertexShaderId);
    glDeleteShader(geometryShaderId);
    glDeleteShader(fragmentShaderId);

    return shaderProgramId;
}
void checkShader(GLuint shaderId, std::string msg ="")
{
    int success;
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);

    std::cout << "The " << (msg.length() > 0 ? msg + " " : "") << "shader compilation was: " << (success ? "successful" : "unsuccessful") << std::endl;

    if (!success)
    {
        char infoLog[512];
        glGetShaderInfoLog(shaderId, 512, NULL, infoLog);
        std::cout << infoLog << std::endl;
    }
}
