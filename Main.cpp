#include <iostream>
#include<fstream>
#include<sstream>
#include<cerrno>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Classes/Shader.h"
#include "Classes/VAO.h"

using namespace std;
using namespace glm;

GLFWwindow* startGLFW();
void calculateTranslations(vec4 translations[], unsigned int numTranslations, float offset);
double displayRefreshRate(double& prev, GLFWwindow* window);

int screenX = 980;
int screenY = 540;
const unsigned int NUM_CIRCLES = 1;

int main(void)
{
    GLFWwindow* window = startGLFW();
    if (window == NULL) {
        return -1;
    }
   
    GLfloat vertices[] =
    {
        /*-0.05f, -0.05f,
         0.05f, -0.05f,
         0.05f,  0.05f,
        -0.05f,  0.05f*/
        -10.0f, -10.0f,
         10.0f, -10.0f,
         10.0f,  10.0f,
        -10.0f,  10.0f
         /*100.0f, 100.0f,
         200.0f, 100.0f,
         200.0f, 200.0f,
         100.0f, 200.0f,*/
    };
    unsigned int indices[] = {
        0, 1, 2,
         2, 3, 0
    };
    float hori = screenX / 2.0f;
    float vert = screenY / 2.0f;
    mat4 projection = ortho(-hori, hori, -vert, vert, -1.0f, 1.0f);
    mat4 model = mat4(1.0f);
    float s = 10.0f;
    model = translate(model, vec3(0.0f, 0.0f, 0.0f));
    model = scale(model, vec3(s, s, 1.0f));
    VAO vao;
    vao.enableVAO();
    vao.bindVBO(vertices, sizeof(vertices));
    vao.bindEBO(indices, sizeof(indices));
    vao.enableAttributePointer();
    vao.disableVAO();

    
    Shader shader("src/Shaders/circleVertex.glsl", "src/Shaders/circleFrag.glsl");

    vec4 translations[NUM_CIRCLES];
    calculateTranslations(translations, NUM_CIRCLES, 20.0f);
    double prev = glfwGetTime();
    while (!glfwWindowShouldClose(window))
    {
        double dt = displayRefreshRate(prev, window);
        float g = 9.81 / ((dt * 10e2) * (dt * 10e2));

        glClear(GL_COLOR_BUFFER_BIT);

        shader.use();
        shader.setVec2f("resolution", (float)screenX, (float)screenY);
        shader.setVec4f("color", 0.3f, 0.5f, 1.0f, 1.0f);
        shader.setMat4f("projection", projection);
        shader.setMat4f("model", model);

        vao.enableVAO();

        glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, NUM_CIRCLES);
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
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
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
void calculateTranslations(vec4 translations[], unsigned int numTranslations, float offset) {
    int index = 0;
    int rows = numTranslations / 10;
    for (int y = rows; y >= -rows; y -= 2)
    {
        for (int x = -10; x < 10; x += 2)
        {
            if (index == numTranslations) {
                break;
            }
            vec4 translation;
            translation.x = (float)x / offset;
            translation.y = (float)y / offset;
            translation.z = 0.0f;
            translation.w = 1.0f;
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