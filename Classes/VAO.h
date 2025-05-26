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
class VAO {
private:
	GLuint vao, vbo, ebo;
public:
	VAO();
	void bindVBO(GLfloat vertices[], size_t size);
	void bindEBO(unsigned int indices[], size_t size);
	void enableVAO();
	void disableVAO();
	void enableAttributePointer();
	void destroy();
};

#endif
