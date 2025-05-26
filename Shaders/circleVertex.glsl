#version 330 core

layout(location = 0) in vec2 aPos;

uniform vec4 color;
uniform float scale;
uniform vec2 offsets[100];

out vec4 circleColor;
out vec2 pos;

void main() {
	vec2 offset = offsets[gl_InstanceID];
	gl_Position = vec4(aPos * scale + offset, 0.0, 1.0);
	circleColor = color;
	pos = aPos;
};