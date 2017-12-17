#include <glad/glad.h>
#include <aurora/squareVao.hpp> 

// generating the triangle
// create 3D points for triangle
static const GLfloat vertices[] = {
    0.5f, 0.5f, 0.0f,  1.0f, 0.0f, 0.0f,
    0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
    -0.5f, -0.5f, 0.0f,0.0f, 0.0f, 1.0f,
    -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f,
};

// create the indices of each triangle
static const GLuint indices[] = {
    0, 1, 3,
    1, 2, 3,
};

GLuint getSquareVertexArray()
{
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
