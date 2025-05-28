#version 330 core

layout(location = 0) in vec2 aPos;
layout(location = 2) in mat4 instanceMatrix;

uniform vec4 color;
uniform mat4 projection;

out vec4 circleColor;
out vec2 pos;

void main() {
	vec4 localPos = projection * instanceMatrix * vec4(aPos, 0.0, 1.0);
	gl_Position = localPos;
	circleColor = color;
	pos = aPos;
};