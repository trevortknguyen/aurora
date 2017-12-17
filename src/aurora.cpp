#include <iostream>
#include <math.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <aurora/shader.hpp>
#include <aurora/shaderSources.hpp>
#include <aurora/squareVao.hpp>

void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    if (key == GLFW_KEY_SPACE)
        std::cout << "Space was pressed.\n";
}

int main()
{
    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
    {
        // init failed
        return 1;
    }

    // set OpenGL to version 4.5
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
   
    // SETUP FOR THE SQUARE
    // generate the shader
    // located in shader.cpp
    GLuint squareShaderProgram= createShaderProgram(squaresVertexShaderSource, squaresFragmentShaderSource);
    // generate VAO for squares
    // located in squareVao.cpp
    GLuint squareVAO = getSquareVertexArray();

    // SETUP FOR THE POINTS


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
        float rotationValue = timeValue;
        trans = glm::rotate(trans, glm::radians(rotationValue), glm::vec3(0.0f, 0.0f, 1.0f));


        // RENDERING THE SQUARE

        // get uniform locations
        int timeValueUniformId = glGetUniformLocation(squareShaderProgram, "timeValue");
        int transformUniformId = glGetUniformLocation(squareShaderProgram, "transform");
        // rendering the objects
        // activate shader program
        // update uniforms
        // bind VAO
        glUseProgram(squareShaderProgram);
        glUniform1f(timeValueUniformId, timeValue);
        glUniformMatrix4fv(transformUniformId, 1, GL_FALSE, glm::value_ptr(trans));
        glBindVertexArray(squareVAO);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // unbind VAO
        // deactivate shader program
        glBindVertexArray(0);
        glUseProgram(0);

        // RENDERING THE POINTS




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
