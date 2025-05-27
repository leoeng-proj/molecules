#ifndef CIRCLE_H
#define CIRCLE_H

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <array>

using namespace std;
using namespace glm;

constexpr float MASS = 1.0f;
constexpr float RADIUS = 10.0f; //scale matrix

class Circle {
private:
	vec2 vel;
	vec2 pos;
public:
	Circle(float x, float y);
	vec2 getVel();
	vec2 getPos();
	void setVel(vec2);
	void setPos(vec2);
	array<GLfloat, 8> getVertices();
};

#endif
