#version 330 core

in vec2 pos;
in float velocity;

out vec4 FragColor;

void main() {
	vec2 coord = pos;
	float len = length(coord);
	if (len > 1.0)
		discard;
	vec4 blue = vec4(0.3f, 0.5f, 1.0f, 1.0f);
	vec4 red = vec4(1.0f, 0.3f, 0.3f, 1.0f);
	float ease = smoothstep(0.0, 1.0, velocity); 
	vec4 finalColor = mix(blue, red, ease);
	FragColor = finalColor;
}