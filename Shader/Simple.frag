#version 400 core

in vec3 color;

out vec4 output;

void main() {
	output = vec4(color, 1);
}