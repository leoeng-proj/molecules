#ifndef VAO_H
#define VAO_H

#include <iostream>
#include<fstream>
#include<sstream>
#include<cerrno>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
/*
* make sure VAO is enabled BEFORE calling either bind function
*/
using namespace std;
using namespace glm;
class VAO {
private:
	GLuint vao, vbo, ebo, matrices, velocities;
	static GLfloat vertices[8];
	static unsigned int indices[6];
public:
	VAO();
	void bindVBO();
	void bindEBO();
	//void bindVBO(GLfloat* vertices, size_t size);
	//void bindEBO(unsigned int* indices, size_t size);
	void bindMatrices(mat4 instanceMatrices[], size_t size, const unsigned int NUM_CIRCLES);
	void updateMatrices(mat4 instanceMatrices[], size_t size);
	void bindVelocities(float velocities[], size_t size, const unsigned int NUM_CIRCLES);
	void updateVelocities(float updatedVelocities[], size_t size);
	void enableVAO();
	void disableVAO();
	void enableAttributePointer();
	void destroy();
};

#endif
