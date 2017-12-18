#include <iostream>
#include <math.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "shaderSources.h"
#include "vaoData.h"
#include "simulation.h"

void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

static bool keys[1000];

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS)
    {
        keys[key] = true;
    }
    else if (action == GLFW_RELEASE) {
        keys[key] = false;
    }

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

    // SETUP FOR THE POINTS
    GLuint pointsShaderProgram = createShaderProgram(pointsVertexShaderSource, pointsGeometryShaderSource, pointsFragmentShaderSource);
    //GLuint pointsShaderProgram = createShaderProgram(pointsVertexShaderSource, pointsFragmentShaderSource);
    GLuint pointsVAO = getGraphVertexArray();
    //GLuint pointsVAO = getPointsVertexArray();
    
    initializeR();
    GLuint particlesVao = getParticlesVao(r);
    

    glm::vec3 eye = glm::vec3(0.0, -2.0, 20.0);

    // define reusable transformation matrix
    // stores camera information
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 projection;
    glm::mat4 transform;

    // main rendering and input loop
    while (!glfwWindowShouldClose(window))
    {
        // clear the screen
        //glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        model = glm::mat4(1.0f);
        view = glm::lookAt(eye, glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 1.0));
        projection = glm::perspective(45.0f, 1.0f * 640 / 480, 0.1f, 1000.0f);
        transform = projection * view * model;

        // RENDERING THE POINTS

        GLuint transformUniform = glGetUniformLocation(pointsShaderProgram, "transform");
        glUseProgram(pointsShaderProgram);
        glUniformMatrix4fv(transformUniform, 1, GL_FALSE, glm::value_ptr(transform));
        //glBindVertexArray(pointsVAO);
        glBindVertexArray(particlesVao);

        glDrawArrays(GL_POINTS, 0, nAtom);

        // UNBIND EVERYTHING
        // unbind VAO
        // deactivate shader program
        glBindVertexArray(0);
        glUseProgram(0);

        // windowing stuff
        // swap the buffers to prevent screen tearing
        // Make sure to poll events after the frame buffer swap
        glfwSwapBuffers(window);
        glfwPollEvents();

        velocityVerletDiscretization();
        std::cout << "Time: " << glfwGetTime() << std::endl;
        updateParticles(particlesVao, r);

        if (keys[GLFW_KEY_UP])
        {
            eye += glm::vec3(0.0, 0.01f, 0.0);
        }
        if (keys[GLFW_KEY_DOWN])
        {
            eye += glm::vec3(0.0, -0.01f, 0.0);
        }

        if (keys[GLFW_KEY_RIGHT])
        {
            eye += glm::vec3(0.01, 0.0, 0.0);
        }
        if (keys[GLFW_KEY_LEFT])
        {
            eye += glm::vec3(-0.01, 0.0, 0.0);
        }

        if (keys[GLFW_KEY_R])
        {
            eye += glm::vec3(0.0, 0.0, -0.01);
        }
        if (keys[GLFW_KEY_F])
        {
            eye += glm::vec3(0.0, 0.0, 0.01);
        }

    }

    // destroy the window
    // close the glfw context
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
