#include <iostream>
#include <math.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <aurora/shader.hpp>

void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        //glfwSetWindowShouldClose(window, GLFW_TRUE);
        std::cout << "You can't escape. \n";
    if (key == GLFW_KEY_SPACE)
        std::cout << "Hello. \n";
}

int main()
{
    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
    {
        // init failed
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);


    GLFWwindow* window = glfwCreateWindow(640, 480, "My Title", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        // window or opengl context creation failed
    }

    glfwSetKeyCallback(window, key_callback);


    glfwMakeContextCurrent(window);

    // OpenGL stuff specifics
    
    // use the gladLoader to load all OpenGL functions
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        std::cout << "OpenGL context did not initalized. Could not load functions." << std::endl;
        return -1;
    }

    int width, height;
    // gets the dimensions and inserts them into the integer pointers
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);


    // prevents screen tearing
    glfwSwapInterval(1);


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

    // generate the shader
    GLuint shaderProgramId = createShaderProgram(vertex_shader_text, fragment_shader_text);

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

    // define reusable transformation matrix
    // stores camera information
    glm::mat4 trans;

    // main rendering and input loop
    while (!glfwWindowShouldClose(window))
    {
        // clear the screen
        glClearColor(0.2f, 0.3f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // rendering stuff
        double time = glfwGetTime();
        float timeValue = sin(time) / 2.0f + 0.5f;
        int timeValueUniformId = glGetUniformLocation(shaderProgramId, "timeValue");

        float rotationValue = timeValue;
        trans = glm::rotate(trans, glm::radians(rotationValue), glm::vec3(0.0f, 0.0f, 1.0f));
        int transformUniformId = glGetUniformLocation(shaderProgramId, "transform");

        // rendering the objects
        // activate shader program
        // update uniforms
        // bind VAO
        glUseProgram(shaderProgramId);
        glUniform1f(timeValueUniformId, timeValue);
        glUniformMatrix4fv(transformUniformId, 1, GL_FALSE, glm::value_ptr(trans));
        glBindVertexArray(vertexArrayId);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // unbind VAO
        // deactivate shader program
        glBindVertexArray(0);
        glUseProgram(0);

        // windowing stuff
        // swap the buffers to prevent screen tearing
        // Make sure to poll events after the frame buffer swap
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // destroy the window
    // close the glfw context
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
