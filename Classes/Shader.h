#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h> // include glad to get all the required OpenGL headers
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;
using namespace glm;
class Shader
{
public:
    // the program ID
    GLuint ID;

    // constructor reads and builds the shader
    Shader(const char* vertexPath, const char* fragmentPath);
    // use/activate the shader
    void use();
    void destroy();
    // utility uniform functions
	void setFloat(const string& name, float v1);
	void setVec2f(const string& name, float v1, float v2);
	void setVec4f(const string& name, float v1, float v2, float v3, float v4);
    void setVec2fv(const string& name, unsigned const int nElems, vec2* vector);
    void setVec4fv(const string& name, unsigned const int nElems, vec4* vector);
    void setMat4f(const string& name, mat4 matrix);
private:
    void startProgram(GLuint* vShader, GLuint* fShader);
    void startShader(GLuint* shader, const char* shaderPath);
    string read(const char* shaderPath);
    void compile(GLuint* shader, const char* shaderCode);

};

#endif