#version 330 core

in vec2 pos;
in vec4 circleColor;

out vec4 FragColor;

void main() {
	vec2 coord = pos;
	float len = length(coord);
	if (len > 1.0)
		discard;
	FragColor = circleColor;
}