#version 330 core

layout(location = 0) in vec2 aPos;

uniform vec4 color;
uniform mat4 projection;
uniform mat4 model;

out vec4 circleColor;
out vec2 pos;

void main() {
	vec4 localPos = projection * model * vec4(aPos, 0.0, 1.0);
	gl_Position = localPos;
	circleColor = color;
	pos = localPos.xy;
	pos = aPos;
};