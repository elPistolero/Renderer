#version 400 core

uniform mat4 mvp;

in vec3 vertex;

out vec3 color;

void main() {
	gl_Position = mvp * vec4(vertex, 1);
	color = vertex;
}