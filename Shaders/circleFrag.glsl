#version 330 core

uniform vec2 resolution;

in vec2 pos;
in vec4 circleColor;

out vec4 FragColor;

void main() {
	float aspect = resolution.x / resolution.y;
	vec2 coord = pos;
	coord.x *= aspect;
	float len = length(coord);
	if (len > 0.3)
		discard;
	FragColor = circleColor;
}