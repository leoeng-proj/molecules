#include <iostream>
#include<fstream>
#include<sstream>
#include<cerrno>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "VAO.h"

GLfloat VAO::vertices[8] = {
		-10.0f, -10.0f,
		10.0f, -10.0f,
		10.0f, 10.0f,
		-10.0f, 10.0f
};
unsigned int VAO::indices[6] = {
	0, 1, 2, 2, 3, 0
};

VAO::VAO() {
	glGenVertexArrays(1, &vao);
}
void VAO::bindVBO() {
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}
void VAO::bindEBO() {
	glBindVertexArray(vao);
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}

void VAO::bindMatrices(mat4 instanceMatrices[], size_t size, const unsigned int NUM_CIRCLES) {
	glBindVertexArray(vao);
	glGenBuffers(1, &matrices);
	glBindBuffer(GL_ARRAY_BUFFER, matrices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(mat4) * NUM_CIRCLES, &instanceMatrices[0], GL_STATIC_DRAW);
}
void VAO::updateMatrices(mat4 updatedMatrices[], size_t size) {
	glBindBuffer(GL_ARRAY_BUFFER, matrices);
	glBufferSubData(GL_ARRAY_BUFFER, 0, size, updatedMatrices);
}
void VAO::enableVAO() {
	glBindVertexArray(vao);
}
void VAO::disableVAO() {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
void VAO::enableAttributePointer() {
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0); //aPos

	glBindBuffer(GL_ARRAY_BUFFER, matrices);
	std::size_t vec4Size = sizeof(glm::vec4); //instanceMatrix
	for (int i = 0; i < 4; i++) {
		glEnableVertexAttribArray(2 + i); // locations 2, 3, 4, 5
		glVertexAttribPointer(2 + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(i * vec4Size));
		glVertexAttribDivisor(2 + i, 1);  // Advance per instance, not per vertex
	}
	glBindVertexArray(0);
}
void VAO::destroy() {
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);
}

//void VAO::bindVBO(GLfloat* vertices, size_t size) {
//	glBindVertexArray(vao);
//	glGenBuffers(1, &vbo);
//	glBindBuffer(GL_ARRAY_BUFFER, vbo);
//	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
//}
//void VAO::bindEBO(unsigned int* indices, size_t size) {
//	glBindVertexArray(vao);
//	glGenBuffers(1, &ebo);
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
//}