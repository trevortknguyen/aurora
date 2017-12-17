#include<string>

#ifndef AURORA_SHADER_H
#define AURORA_SHADER_H

extern const char* vertex_shader_text;
extern const char* fragment_shader_text;

void checkShader(GLuint shaderId, std::string msg);

#endif
