#version 400 core

uniform mat4 modelview;
uniform mat4 projection;
uniform mat4 mvp;
uniform mat3 normalMatrix;
uniform vec4 lightPosition;

in vec3 vertex;
in vec3 vertexNormal;
in vec2 texCoord;

out vec3 lightIntensity;

const vec3 Kd = vec3(0.9f, 0.5f, 0.3f);
const vec3 Ka = vec3(0.9f, 0.5f, 0.3f);
const vec3 Ks = vec3(0.8f);
const vec3 Ld = vec3(1.0f);
const vec3 La = vec3(0.4f);
const vec3 Ls = vec3(1.0f);
const float shininess = 100.0f;

void main() {
    vec3 tnorm = normalize(normalMatrix * vertexNormal);
    vec4 eyeCoords = modelview * vec4(vertex, 1.0f);
    vec3 s = normalize(vec3(lightPosition - eyeCoords));
    vec3 v = normalize(-eyeCoords.xyz);
    vec3 r = reflect(-s, tnorm);
    float sDotN = max(dot(s, tnorm), 0.0f);
    vec3 ambient = La * Ka;
    vec3 diffuse = Ld * Kd * sDotN;
    vec3 spec = vec3(0.0f);
    if (sDotN > 0.0f)
        spec = Ls * Ks * pow(max(dot(r, v), 0.0f), shininess);
        
    lightIntensity = ambient + diffuse + spec;

	gl_Position = mvp * vec4(vertex, 1);
}