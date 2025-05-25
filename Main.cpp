#include <iostream>
#include<fstream>
#include<sstream>
#include<cerrno>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

const char* circleVertex = "#version 330 core\n"
"layout(location = 0) in vec3 aPos;\n"
"uniform vec4 color;\n"
"uniform float scale;\n"
"out vec4 circleColor;\n"
"out vec3 pos;"
"void main() {\n"
"   gl_Position = vec4(aPos * scale, 1.0);\n"
"   circleColor = color;\n"
"   pos = aPos;\n"
"}\n\0";
const char* circleFrag = "#version 330 core\n"
"in vec3 pos;\n"
"in vec4 circleColor;\n"
"out vec4 FragColor;\n"
"uniform vec2 resolution;\n"
"void main() {\n"
"   float aspect = resolution.x / resolution.y;\n" 
"   float len = length(pos);\n"
"   if (len > 1.0)\n"
"       discard;\n"
"   FragColor = circleColor;\n"
"}\n\0";
const char* circleShaderSource = "#version 330 core\n"
"uniform vec2 resolution;\n"
"in vec2 FragCoord;"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   vec2 fragCoord = gl_FragCoord.xy;\n"
"   vec2 uv = fragCoord / iResolution.xy * 2.0 - 1.0;\n"
"   float aspect = resolution.x / resolution.y;\n"
"   uv.x *= aspect;\n"
"   float distance = 1 - length(uv);\n"
"   distance = ceil(distance);\n"
"   FragColor.rgb = vec3(distance);\n"
"   FragColor.rgb *= vec3(0.3, 0.5, 1);\n"
"}\n\0";
using namespace std;

double displayRefreshRate(double& prev, GLFWwindow* window);
GLFWwindow* startGLFW();
void startShader(GLuint* shader, const char* shaderCode);
int screenX = 1000;
int screenY = 800;
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
       1.0f, 1.0f, 0.0f,
       1.0f, -1.0f, 0.0f,
       -1.0f, -1.0f, 0.0f,
       -1.0f, 1.0f, 0.0f,
    };
    unsigned int indices[] = {
    0, 1, 3,
    1, 2, 3
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    // Enable the Vertex Attribute so that OpenGL knows to use it
    glEnableVertexAttribArray(0);
    //bind everything to 0 so changes are done
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    double prev = glfwGetTime();  // Set the initial 'previous time'.
    while (!glfwWindowShouldClose(window))
    {
        double dt = displayRefreshRate(prev, window);
        glClear(GL_COLOR_BUFFER_BIT);

        int color = glGetUniformLocation(program, "color");
        glUniform4f(color, 0.3, 0.5, 1.0, 1.0);
        int scale = glGetUniformLocation(program, "scale");
        glUniform1f(scale, 0.05);
        int iRes = glGetUniformLocation(program, "iResolution");
        glUniform2f(iRes, screenX, screenY);
        glUseProgram(program);
        // Bind the VAO so OpenGL knows to use it
        glBindVertexArray(VAO);
        // Draw the triangle using the GL_TRIANGLES primitive
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
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