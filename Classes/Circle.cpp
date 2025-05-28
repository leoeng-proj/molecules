#include "Circle.h"

using namespace std;

Circle::Circle() : pos(vec2(0.0f, 0.0f)), vel(vec2(0)) {
}
Circle::Circle(vec2 p) : pos(vec2(p.x, p.y)), vel(vec2(0)) {
}
vec2 Circle::getVel() {
	return vel;
}
vec2 Circle::getPos() {
	return pos;
}
mat4 Circle::getMatrix() {
	return modelMatrix;
}
void Circle::setVel(vec2 v) {
	vel = v;
}
void Circle::setPos(vec2 p) {
	pos = p;
}
void Circle::updatePos(float dt, vec2 dim) {
	checkBounds(dim);
	pos += vel * dt;
}
void Circle::setMatrix(mat4 mat) {
	modelMatrix = mat;
}
array<GLfloat, 8> Circle::getVertices() {
	array<GLfloat, 8> arr = {
		pos.x - RADIUS, pos.y - RADIUS, //quadrant 3
		pos.x + RADIUS, pos.y - RADIUS,  //quadrant 4
		pos.x + RADIUS, pos.y + RADIUS, //quadrant 1
		pos.x - RADIUS, pos.y + RADIUS //quadrant 2
		//indices: 0, 1, 2, 2, 3, 0 for quad
	};
	return arr;
}
void Circle::gravity(float dt) {
	vel.y -= GRAVITY * dt;
}
void Circle::checkBounds(vec2 dim) {
	float damping = 0.7f;
	vec2 bounds = dim * 0.5f - vec2(RADIUS);
	if (abs(pos.x) > bounds.x) {
		pos.x = bounds.x * sign(pos.x);
		vel *= -1 * damping;
	}
	if (abs(pos.y) > bounds.y) {
		pos.y = bounds.y * sign(pos.y);
		vel *= -1 * damping;
	}
}