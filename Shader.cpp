#include <glad/glad.h> // include glad to get all the required OpenGL headers
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"

using namespace std;
using namespace glm;

//public
Shader::Shader(const char* vertexPath, const char* fragmentPath) {
	GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
	startShader(&vShader, vertexPath);
	GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);
	startShader(&fShader, fragmentPath);
	ID = glCreateProgram();
	startProgram(&vShader, &fShader);
};
void Shader::use() {
	glUseProgram(ID);
};
void Shader::destroy() {
	glDeleteProgram(ID);
};
void Shader::setFloat(const string& name, float v1) {
	int ptr = glGetUniformLocation(ID, name.c_str());
	glUniform1f(ptr, v1);
};
void Shader::setVec2f(const string& name, float v1, float v2) {
	int ptr = glGetUniformLocation(ID, name.c_str());
	glUniform2f(ptr, v1, v2);
};
void Shader::setVec4f(const string& name, float v1, float v2, float v3, float v4) {
	int ptr = glGetUniformLocation(ID, name.c_str());
	glUniform4f(ptr, v1, v2, v3, v4);
};
void Shader::setVec2fv(const string& name, unsigned const int nElems, vec2* vector) {
	int ptr = glGetUniformLocation(ID, name.c_str());
	glUniform2fv(ptr, nElems, value_ptr(*vector));
};

//private
void Shader::startProgram(GLuint* vShader, GLuint* fShader) {
	glAttachShader(ID, *vShader);
	glAttachShader(ID, *fShader);
	glLinkProgram(ID);
	glDeleteShader(*vShader);
	glDeleteShader(*fShader);
}
void Shader::startShader(GLuint* shader, const char* shaderPath) {
	string source = read(shaderPath);      
	const char* shaderCode = source.c_str();
	compile(shader, shaderCode);
}
string Shader::read(const char* shaderPath) {
	// 1. retrieve the vertex/fragment source code from filePath
	std::string shaderCode;
	std::ifstream shaderFile;
	// ensure ifstream objects can throw exceptions:
	shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		// open files
		shaderFile.open(shaderPath);
		std::stringstream shaderStream;
		// read file's buffer contents into streams
		shaderStream << shaderFile.rdbuf();
		// close file handlers
		shaderFile.close();
		// convert stream into string
		shaderCode = shaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	return shaderCode;
}
void Shader::compile(GLuint* shader, const char* shaderCode) {
	glShaderSource(*shader, 1, &shaderCode, NULL);
	glCompileShader(*shader);
	int  success;
	char infoLog[512];
	glGetShaderiv(*shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(*shader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
}