#ifndef AURORA_VAO_DATA_H
#define AURORA_VAO_DATA_H

#include<glad/glad.h>

void updateParticles(GLuint vao, float r [][3]);
GLuint getParticlesVao(float r [][3]);

GLuint getGraphVertexArray();
GLuint getPointsVertexArray();
GLuint getSquareVertexArray();

#endif
