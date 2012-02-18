#version 400 core

uniform mat4 modelview;
uniform mat4 projection;
uniform mat4 mvp;
uniform mat3 normalMatrix;

in vec3 vertex;
in vec3 vertexNormal;
in vec2 texCoord;

out vec3 color;

void main() {
	gl_Position = mvp * vec4(vertex, 1);
	color = vertex;
}