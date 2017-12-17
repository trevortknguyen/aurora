#include <iostream>
#include <math.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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

    // generate the vertex shader

    const char* vertex_shader_text = R"glsl(
        #version 450 core
        layout (location = 0) in vec3 aPos;
        layout (location = 1) in vec3 aCol;
        
        out vec3 color;
        uniform float timeValue;
        uniform mat4 transform;

        vec3 genColor(in float t, in vec3 c, in vec3 p, in vec3 pt)
        {
            vec3 a;
            vec3 b;
            a = vec3(t*c.x+(1-t)*c.y,(1-t)*c.x+t*c.y,0.6*t*c.x+0.2*(1-c.y));
            b = vec3(t*pt.x+(1-t)*c.x,(1-t)*pt.y+t*c.y, 0.6*t+0.2*(1-p.y));

            if (t > 0.9)
            {
                if (int(t * 1000000) % 2 == 0)
                {
                    return a;
                }
                else
                {
                    return b;
                }
            }
 
            vec3 color;
            color = vec3(t*a.r+(1-t)*b.r, t*a.g+(1-t)*b.g, t*a.b+(1-t)*b.b);
            return color;
        }
        
        void main()
        {
            gl_Position = transform*vec4(aPos.xyz, 1.0);
            color = genColor(timeValue, aCol, aPos, gl_Position.xyz);
        }
    )glsl";

    GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShaderId, 1, &vertex_shader_text, NULL);
    glCompileShader(vertexShaderId);

    int success;
    glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &success);

    std::cout << "The vertex shader compilation was: " << (success ? "successful" : "unsuccessful") << std::endl;

    if (!success)
    {
        char infoLog[512];
        glGetShaderInfoLog(vertexShaderId, 512, NULL, infoLog);
        std::cout << infoLog << std::endl;
    }


    // generate the fragment shader
    const char* fragment_shader_text = R"glsl(
        #version 450 core
        out vec4 fragColor;
    
        in vec3 color;
    
        void main()
        {
            fragColor = vec4(color.rgb, 1.0f);
        }
    )glsl";

    GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShaderId, 1, &fragment_shader_text, NULL);
    glCompileShader(fragmentShaderId);

    glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &success);
    std::cout << "The fragment shader compilation was: " << (success ? "successful" : "unsuccessful") << std::endl;

    // generate the shader program
    GLuint shaderProgramId = glCreateProgram();

    glAttachShader(shaderProgramId, vertexShaderId);
    glAttachShader(shaderProgramId, fragmentShaderId);
    glLinkProgram(shaderProgramId);

    // program is linked
    // individual shaders are not needed
    glDeleteShader(vertexShaderId);
    glDeleteShader(fragmentShaderId);


    // vertex array object contains the buffers
    GLuint vertexArrayId;
    // store the vertex array id of one new array
    glGenVertexArrays(1, &vertexArrayId);
    // set the current vertex array
    glBindVertexArray(vertexArrayId);

    // vertex buffer objects
    GLuint vertexBufferId;
    glGenBuffers(1, &vertexBufferId);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
    //load in the triangle points into the vertex buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // define schema to read buffer
    // use the stride because they are processed with layout
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // element buffers
    GLuint elementBufferId;
    glGenBuffers(1, &elementBufferId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


    glBindVertexArray(0);

    glm::mat4 trans;

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.2f, 0.3f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // rendering stuff
        
        double time = glfwGetTime();
        float timeValue = sin(time) / 2.0f + 0.5f;
        int timeValueUniformId = glGetUniformLocation(shaderProgramId, "timeValue");

        float rotationValue = timeValue;
        trans = glm::rotate(trans, glm::radians(rotationValue), glm::vec3(0.0f, 0.0f, 1.0f));
        int transformUniformId = glGetUniformLocation(shaderProgramId, "transform");

        glUseProgram(shaderProgramId);
        // update uniforms
        glUniform1f(timeValueUniformId, timeValue);
        glUniformMatrix4fv(transformUniformId, 1, GL_FALSE, glm::value_ptr(trans));
        glBindVertexArray(vertexArrayId);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);
        glUseProgram(0);

        // windowing stuff

        glfwSwapBuffers(window);

        // Make sure to poll events after the frame buffer swap
        glfwPollEvents();
    }

    glfwDestroyWindow(window);

    glfwTerminate();

    return 0;
}
