#ifndef CIRCLE_H
#define CIRCLE_H

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <array>
#include <cstdlib>

using namespace std;
using namespace glm;

constexpr float MASS = 1.0f;
constexpr float RADIUS = 10.0f; //scale matrix
constexpr float GRAVITY = 5.0f; //scale matrix

class Circle {
private:
	vec2 vel;
	vec2 pos;
	void checkBounds(vec2);
public:
	Circle();
	Circle(vec2);
	vec2 getVel();
	vec2 getPos();
	void setVel(vec2);
	void setPos(vec2);
	void updatePos(float, vec2);
	void gravity(float);
};

#endif
