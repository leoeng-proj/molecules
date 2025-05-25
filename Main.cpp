#include <iostream>
#include<fstream>
#include<sstream>
#include<cerrno>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

const char* circleVertex = "#version 330 core\n"
"layout(location = 0) in vec2 aPos;\n"
"uniform vec4 color;\n"
"uniform float scale;\n"
"uniform vec2 offsets[100];"
"out vec4 circleColor;\n"
"out vec2 pos;"
"void main() {\n"
"   vec2 offset = offsets[gl_InstanceID];"
"   gl_Position = vec4(aPos * scale + offset, 0.0, 1.0);"
"   circleColor = color;\n"
"   pos = aPos;\n"
"}\n\0";
const char* circleFrag = "#version 330 core\n"
"uniform vec2 resolution;\n"
"in vec2 pos;\n"
"in vec4 circleColor;\n"
"out vec4 FragColor;\n"
"void main() {\n"
"   float aspect = resolution.x / resolution.y;\n" 
"   vec2 coord = pos;\n"
"   coord.x *= aspect;\n"
"   float len = length(coord);\n"
"   if (len > 0.05)\n"
"       discard;\n"
"   FragColor = circleColor;\n"
"}\n\0";

using namespace std;
using namespace glm;
double displayRefreshRate(double& prev, GLFWwindow* window);
GLFWwindow* startGLFW();
void startShader(GLuint* shader, const char* shaderCode);
float screenX = 1000.0f;
float screenY = 800.0f;
int main(void)
{
    //initialization
    GLFWwindow* window = startGLFW();
    if (window == NULL) {
        return -1;
    }

    //shader work
    GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
    startShader(&vShader, circleVertex);
    GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);
    startShader(&fShader, circleFrag);
    GLuint program = glCreateProgram();
    glAttachShader(program, vShader);
    glAttachShader(program, fShader);
    glLinkProgram(program);
    glDeleteShader(vShader);
    glDeleteShader(fShader);


    GLfloat vertices[] =
    {
       /*1.0f, 1.0f, 
       1.0f, -1.0f, 
       -1.0f, -1.0f, 
       -1.0f, 1.0f*/
        -0.05f, -0.05f,
         0.05f, -0.05f,
         0.05f,  0.05f,
        -0.05f,  0.05f
    };
    unsigned int indices[] = {
    /*    0, 1, 3,
        1, 2, 3*/
        0, 1, 2,
         2, 3, 0
    };
    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    //stage the VAO
    glBindVertexArray(VAO);
    //bind VBO to VAO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //vertices into VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    //indices into EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    // Configure the Vertex Attribute so that OpenGL knows how to read the VBO
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    // Enable the Vertex Attribute so that OpenGL knows to use it
    glEnableVertexAttribArray(0);
    //bind everything to 0 so changes are done
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    vec2 translations[100];
    int index = 0;
    float offset = 0.1f;
    for (int y = -10; y < 10; y += 2)
    {
        for (int x = -10; x < 10; x += 2)
        {
            vec2 translation;
            translation.x = (float)x / 20.0f + offset;
            translation.y = (float)y / 20.0f + offset;
            translations[index++] = translation;
        }
    }

    double prev = glfwGetTime();  // Set the initial 'previous time'.
    while (!glfwWindowShouldClose(window))
    {
        double dt = displayRefreshRate(prev, window);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(program);
        int color = glGetUniformLocation(program, "color");
        glUniform4f(color, 0.3, 0.5, 1.0, 1.0);
        int scale = glGetUniformLocation(program, "scale");
        glUniform1f(scale, 0.5);
        int res = glGetUniformLocation(program, "resolution");
        glUniform2f(res, screenX, screenY);
        int offsets = glGetUniformLocation(program, "offsets");
        glUniform2fv(offsets, 100, value_ptr(translations[0]));
        // Bind the VAO so OpenGL knows to use it
        glBindVertexArray(VAO);
        // Draw the triangle using the GL_TRIANGLES primitive
        glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, 100);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    //Close things up
    glDeleteProgram(program);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    // Delete window before ending the program
    glfwDestroyWindow(window);
    // Terminate GLFW before ending the program
    glfwTerminate();
    return 0;
}
GLFWwindow* startGLFW() {
    GLFWwindow* window;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
    if (!glfwInit())
        return NULL;
    window = glfwCreateWindow(screenX, screenY, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return NULL;
    }
    glfwMakeContextCurrent(window);
    gladLoadGL();
    glViewport(0, 0, screenX, screenY);
    return window;
}
void startShader(GLuint* shader, const char* shaderCode) {
    glShaderSource(*shader, 1, &shaderCode, NULL);
    glCompileShader(*shader);
    int  vsuccess;
    char vinfoLog[512];
    glGetShaderiv(*shader, GL_COMPILE_STATUS, &vsuccess);
    if (!vsuccess)
    {
        glGetShaderInfoLog(*shader, 512, NULL, vinfoLog);
        std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << vinfoLog << std::endl;
    }
}
double displayRefreshRate(double& prev, GLFWwindow* window) {
    double curr = glfwGetTime();   // Get the current time.
    double dt = curr - prev; // Work out the time elapsed over the last frame.
    prev = curr;          // Set the 'previous time' for the next frame to use.
    if (dt > 0.0) {
        double fps = 1.0 / dt;
        // Create a string and put the FPS as the window title.
        char tmp[256];
        sprintf(tmp, "FPS %.2lf", fps);
        glfwSetWindowTitle(window, tmp);
    }
    return dt;
}