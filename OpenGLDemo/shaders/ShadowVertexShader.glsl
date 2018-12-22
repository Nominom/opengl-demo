#version 330 core
layout(location=0) in vec3 vertexPosition_modelspace;
layout(location=1) in vec2 uvcoords;

uniform mat4 lightSpaceMatrix;
uniform mat4 model;

out vec2 uv;

void main() {
    gl_Position = lightSpaceMatrix * model * vec4(vertexPosition_modelspace, 1.0);
	uv = uvcoords;
}  