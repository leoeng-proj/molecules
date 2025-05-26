#include <iostream>
#include<fstream>
#include<sstream>
#include<cerrno>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"
#include "VAO.h"

using namespace std;
using namespace glm;

GLFWwindow* startGLFW();
void calculateTranslations(vec2 translations[], unsigned int numTranslations);
double displayRefreshRate(double& prev, GLFWwindow* window);
float screenX = 1000.0f;
float screenY = 800.0f;

int main(void)
{
    //initialization
    GLFWwindow* window = startGLFW();
    if (window == NULL) {
        return -1;
    }
   
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
    VAO vao;
    vao.enableVAO();
    vao.bindVBO(vertices, sizeof(vertices));
    vao.bindEBO(indices, sizeof(indices));
    vao.enableAttributePointer();
    vao.disableVAO();

    
    Shader shader("src/circleVertex.glsl", "src/circleFrag.glsl");

    const unsigned int n = 100;
    vec2 translations[n];
    calculateTranslations(translations, n);
    double prev = glfwGetTime();  // Set the initial 'previous time'.
    while (!glfwWindowShouldClose(window))
    {
        double dt = displayRefreshRate(prev, window);
        glClear(GL_COLOR_BUFFER_BIT);

        shader.use();
        shader.setFloat("scale", 0.5);
        shader.setVec2f("resolution", screenX, screenY);
        shader.setVec4f("color", 0.3, 0.5, 1.0, 1.0);
        shader.setVec2fv("offsets", n, &translations[0]);
        // Bind the VAO so OpenGL knows to use it
        vao.enableVAO();
        // Draw the triangle using the GL_TRIANGLES primitive
        glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, 100);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    //Close things up
    shader.destroy();
    vao.destroy();
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
void calculateTranslations(vec2 translations[], unsigned int numTranslations) {
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