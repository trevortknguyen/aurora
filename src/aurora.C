#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

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


    // prevents screen tearing
    glfwSwapInterval(1);


    // generating the triangle
    
    // create 3D points for triangle
    static const GLfloat vertices[] = {
        0.5f, 0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        -0.5f, 0.5f, 0.0f,
    };

    // create the indices of each triangle
    static const GLuint indices[] = {
        0, 1, 3,
        1, 2, 3,
    };

    // generate the vertex shader

    const char* vertex_shader_text =
        "#version 450 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "\n"
        "out vec4 vertexColor;\n"
        "\n"
        "void main()\n"
        "{\n"
        "    gl_Position = vec4(aPos.xyz, 1.0);\n"
        "    vertexColor = vec4(0.2*aPos.x + 0.8*aPos.y, 0.3*aPos.y+0.7*aPos.x, 0.5-0.5*aPos.x, 1.0);\n"
        "}\n";

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
    const char* fragment_shader_text =
        "#version 450 core\n"
        "out vec4 fragColor;\n"
        "\n"
        "in vec4 vertexColor;\n"
        "\n"
        "void main()\n"
        "{\n"
        //"    fragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
        "    fragColor = vertexColor;\n"
        "}\n";

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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // element buffers
    GLuint elementBufferId;
    glGenBuffers(1, &elementBufferId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


    glBindVertexArray(0);

    while (!glfwWindowShouldClose(window))
    {
        double time = glfwGetTime();
        //std::cout << "Time: " << time << ".\n";

        // rendering stuff
        
        glViewport(0, 0, width, height);

        glUseProgram(shaderProgramId);
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
