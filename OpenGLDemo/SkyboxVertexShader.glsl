#version 330 core
layout (location = 0) in vec3 vertexPosition_modelspace;

out vec3 TexCoords;

uniform mat4 projection;
uniform mat4 view;

void main() {
    TexCoords = vertexPosition_modelspace;
    gl_Position = projection * view * vec4(vertexPosition_modelspace, 1.0);
}