#include <iostream>
#include<fstream>
#include<sstream>
#include<cerrno>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <array>
#include "Classes/Shader.h"
#include "Classes/VAO.h"
#include "Classes/Circle.h"

using namespace std;
using namespace glm;

GLFWwindow* startGLFW();
void calculateTranslations(vec4 translations[], unsigned int numTranslations, float offset);
float displayRefreshRate(float& prev, GLFWwindow* window);

int screenX = 980;
int screenY = 540;
const unsigned int NUM_CIRCLES = 2;
float g = 5.0f;

int main(void)
{
    GLFWwindow* window = startGLFW();
    if (window == NULL) {
        return -1;
    }

 
    Circle c1(0, 0);
    array<GLfloat, 8> vertices = c1.getVertices();
    unsigned int indices[] = {
        0, 1, 2, 2, 3, 0
    };
    
    GLfloat allVertices[NUM_CIRCLES * 8];
    unsigned int allIndices[NUM_CIRCLES * 6];
    int k = 0;
    for (int i = 0; i < NUM_CIRCLES; i++) {
        Circle c(0.0f + i * 20.0f, 0.0f + i * 20.0f);
        array<GLfloat, 8> vertices = c.getVertices();
        for (int j = 0; j < 8; j++) {
            allVertices[j + i * 8] = vertices.data()[j];
        };
        allIndices[k] = k;
        allIndices[k + 1] = k + 1;
        allIndices[k + 2] = k + 2;
        allIndices[k + 3] = k + 2;
        allIndices[k + 4] = k + 3;
        allIndices[k + 5] = k;
        k += 6;
    };

    VAO vao;
    vao.enableVAO();
    vao.bindVBO(allVertices, sizeof(allVertices));
    vao.bindEBO(allIndices, sizeof(allIndices));
    vao.enableAttributePointer();
    vao.disableVAO();

    
    Shader shader("src/Shaders/circleVertex.glsl", "src/Shaders/circleFrag.glsl");

    float hori = screenX / 2.0f;
    float vert = screenY / 2.0f;
    mat4 projection = ortho(-hori, hori, -vert, vert, -1.0f, 1.0f);

    float prev = (float)glfwGetTime();
    while (!glfwWindowShouldClose(window))
    {

        glClear(GL_COLOR_BUFFER_BIT);
        float dt = displayRefreshRate(prev, window);
        dt *= 10;
        vec2 newVel = c1.getVel() - vec2(0.0f, (g * dt));

        c1.setVel(newVel);
        float s = 10.0f;
        if (c1.getPos().y < -vert + s) {
            c1.setVel(c1.getVel() * -1.0f);
        }
        c1.setPos(c1.getPos() + c1.getVel() * dt);
        mat4 model = mat4(1.0f);
        model = translate(model, vec3(c1.getPos(), 0.0f));
        model = scale(model, vec3(RADIUS, RADIUS, 1.0f));
 

        shader.use();
        //shader.setVec2f("resolution", (float)screenX, (float)screenY);
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
float displayRefreshRate(float& prev, GLFWwindow* window) {
    float curr = (float)glfwGetTime();   // Get the current time.
    float dt = curr - prev; // Work out the time elapsed over the last frame.
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