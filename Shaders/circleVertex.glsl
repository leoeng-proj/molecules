#version 330 core

layout(location = 0) in vec2 aPos;
layout(location = 2) in float aVel;
layout(location = 3) in mat4 instanceMatrix;

uniform mat4 projection;

out float velocity;
out vec2 pos;

void main() {
	vec4 localPos = projection * instanceMatrix * vec4(aPos, 0.0, 1.0);
	gl_Position = localPos;
	velocity = aVel;
	pos = aPos;
};