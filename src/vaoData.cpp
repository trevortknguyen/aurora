#include <glad/glad.h>
#include <cmath>

#include "vaoData.h"

void updateParticles(GLuint vao, float rvalues [][3])
{
    glBindVertexArray(vao);

    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float)*2000*3, rvalues);

    glBindVertexArray(0);
}

// need to find out how to fix that
GLuint getParticlesVao(float rvalues [][3])
{
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*2000*3, rvalues, GL_STREAM_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
    return vao;
}


GLuint getGraphVertexArray()
{
    float graph[2000][3];

    for (int i = 0; i < 2000; i++) {
        float t = (i-1000.0) / 100.0;
        graph[i][0] = 0.1 * cos(t * 10.0); 
        graph[i][1] = 0.1 * sin(t * 10.0);
        graph[i][2] = 0.1*t;
    }

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(graph), graph, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
    return vao;
}

GLuint getPointsVertexArray()
{
    float vertices[3][3] = {
        0.5f, 0.0f, 0.0f,
        0.0f, 0.5f, 0.0f,
        0.0f, 0.0f, 0.0f,
    };

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
    return vao;
}

GLuint getSquareVertexArray()
{
    // generating the triangle
    // create 3D points for triangle
    const GLfloat vertices[] = {
        0.5f, 0.5f, 0.0f,  1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.0f,0.0f, 0.0f, 1.0f,
        -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f,
    };

    // create the indices of each triangle
    const GLuint indices[] = {
        0, 1, 3,
        1, 2, 3,
    };

    // vertex array object (VAO) contains buffers and attribute pointers
    // create new VAO and store pointer
    // bind the VAO
    GLuint vertexArrayId;
    glGenVertexArrays(1, &vertexArrayId);
    glBindVertexArray(vertexArrayId);

    // vertex buffer object (VBO) store buffered data
    // create new VBO and store pointer
    // bind the VBO
    // load in the triangle points into the VBO
    GLuint vertexBufferId;
    glGenBuffers(1, &vertexBufferId);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // vertex attrib pointers define how to read buffer
    // define (location=0)
    // stores the points
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // vertex attrib pointers define how to read buffer
    // define (location=1)
    // stores the colors
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // element buffer object (EBO) stores indices
    // create new EBO and store pointer
    // bind the EBO
    // load in the indices into the EBO
    GLuint elementBufferId;
    glGenBuffers(1, &elementBufferId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // keep the VBOs and EBO bound
    // unbind the VAO
    glBindVertexArray(0);

    return vertexArrayId;
}

