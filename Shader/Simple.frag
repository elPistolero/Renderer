#version 400 core

in vec3 lightIntensity;

out vec4 output;

void main() {
	output = vec4(lightIntensity, 1);
}