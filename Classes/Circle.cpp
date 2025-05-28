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
void Circle::collisions(Circle circles[], const unsigned int NUM_CIRCLES) {
	for (int i = 0; i < NUM_CIRCLES; i++) {
		if (&circles[i] == this) {
			continue;
		}
		vec2 collisionAxes = pos - circles[i].getPos();
		float dist = length(collisionAxes);
		if (dist >= RADIUS * 2) {
			continue;
		}
		vec2 n = collisionAxes / dist;
		float delta = RADIUS * 2 - dist;
		pos += delta * n;
		circles[i].setPos(circles[i].getPos() - delta * n);
	}
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