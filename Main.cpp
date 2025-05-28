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
void placeCircles(Circle circles[], mat4 modelMatrices[], float offset);
float displayRefreshRate(float& prev, GLFWwindow* window);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void runUpdates(Circle circles[], mat4 modelMatrices[], float velocities[], float dt);
vec2 dim(980.0f, 540.0f);
const unsigned int NUM_CIRCLES = 100;
float g = 5.0f;
bool paused = true;

int main(void)
{
    GLFWwindow* window = startGLFW();
    if (window == NULL) {
        return -1;
    }

    Circle circles[NUM_CIRCLES];
    mat4 modelMatrices[NUM_CIRCLES];
    float velocities[NUM_CIRCLES]{};
    placeCircles(circles, modelMatrices, 40.0f);
    
    VAO vao;
    vao.enableVAO();
    vao.bindVBO();
    vao.bindEBO();
    vao.bindMatrices(modelMatrices, sizeof(modelMatrices), NUM_CIRCLES);
    vao.bindVelocities(velocities, sizeof(velocities), NUM_CIRCLES);
    vao.enableAttributePointer();
    vao.disableVAO();

    Shader shader("src/Shaders/circleVertex.glsl", "src/Shaders/circleFrag.glsl");

    float hori = dim.x / 2.0f;
    float vert = dim.y / 2.0f;
    mat4 projection = ortho(-hori, hori, -vert, vert, -1.0f, 1.0f);
    float prev = (float)glfwGetTime();
    while (!glfwWindowShouldClose(window)){
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT);
        float dt = displayRefreshRate(prev, window);
        dt *= 10;
        if (!paused) {
            runUpdates(circles, modelMatrices, velocities, dt);
        }
        
        vao.updateMatrices(modelMatrices, sizeof(modelMatrices));
        vao.updateVelocities(velocities, sizeof(velocities));
        shader.use();
        //shader.setFloat("vel", 1.0f, 0.3f, 0.3f, 1.0f);
        shader.setMat4f("projection", projection);

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
    window = glfwCreateWindow((int)dim.x, (int)dim.y, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return NULL;
    }
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, keyCallback);
    gladLoadGL();
    glViewport(0, 0, (int)dim.x, (int)dim.y);
    return window;
}
void placeCircles(Circle circles[], mat4 modelMatrices[], float offset) {
    int numCols = 10;  
    int numRows = NUM_CIRCLES / numCols;  
    float startX = -((numCols - 1) * offset) / 2.0f;
    float startY = -((numRows - 1) * offset) / 2.0f;
    int i = 0;
    for (int row = 0; row < numRows; ++row) {
        for (int col = 0; col < numCols; ++col) {
            if (i == NUM_CIRCLES) break;
            float x = startX + col * offset;
            float y = startY + row * offset;
            circles[i].setPos(vec2(x, y));
            mat4 model = mat4(1.0f);
            model = translate(model, vec3(x, y, 0));
            model = scale(model, vec3(RADIUS, RADIUS, 1.0f));
            modelMatrices[i] = model;
            i++;
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
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        paused = !paused;
    }
}
void runUpdates(Circle circles[], mat4 modelMatrices[], float velocities[], float dt) {
    for (int i = 0; i < NUM_CIRCLES; ++i) {
        circles[i].gravity(dt);
        circles[i].updatePos(dt, dim);
        velocities[i] = dot(circles[i].getVel(), circles[i].getVel()) / 10e2;
        modelMatrices[i] = mat4(1.0f);
        modelMatrices[i] = translate(modelMatrices[i], vec3(circles[i].getPos(), 0.0f));
        modelMatrices[i] = scale(modelMatrices[i], vec3(RADIUS, RADIUS, 1.0f));
    }
}