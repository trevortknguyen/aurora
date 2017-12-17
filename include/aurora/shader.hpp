#include<string>

#ifndef AURORA_SHADER_H
#define AURORA_SHADER_H

GLuint createShaderProgram(const char* vertexSource, const char* fragmentSource);

void checkShader(GLuint shaderId, std::string msg);

#endif
