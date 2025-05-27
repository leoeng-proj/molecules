#include "Circle.h"

using namespace std;

Circle::Circle(float x, float y) : pos(vec2(x, y)), vel(vec2(0)) {
}
vec2 Circle::getVel() {
	return vel;
}
vec2 Circle::getPos() {
	return pos;
}
void Circle::setVel(vec2 v) {
	vel = v;
}
void Circle::setPos(vec2 p) {
	pos = p;
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