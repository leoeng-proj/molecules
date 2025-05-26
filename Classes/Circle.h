#ifndef CIRCLE_H
#define CIRCLE_H

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

using namespace std;
using namespace glm;

static const float MASS = 10;
static const float RADIUS = 1;

class Circle {
private:
	vec2 pos;
public:
	Circle();
	Circle(float x, float y, float r);

};

#endif
