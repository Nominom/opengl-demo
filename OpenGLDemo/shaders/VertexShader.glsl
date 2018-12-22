#version 330 core

struct Light{
	vec3 direction;
	vec3 color;
};

layout(location=0) in vec3 vertexPosition_modelspace;
layout(location=1) in vec2 uvcoords;
layout(location=2) in vec3 normals;
layout(location=3) in vec3 tangent;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceMatrix;
uniform mat3 normalMat;

out vec2 uv;
out vec3 o_normal;
out vec3 fragPos;
out vec4 fragPosLightSpace;

//out mat3 TBN;
out vec3 tangentLightDir;
out vec3 tangentCamPos;
out vec3 tangentFragPos;

uniform Light light;
uniform vec3 cameraPos;

void main(){
	
	gl_Position = projection * view * model * vec4(vertexPosition_modelspace, 1);

	fragPos = vec3(model * vec4(vertexPosition_modelspace, 1));
	fragPosLightSpace = lightSpaceMatrix * vec4(fragPos, 1.0);

	uv = uvcoords;
	o_normal = normalMat * normals;

    vec3 T = normalize(normalMat * tangent);
    vec3 N = normalize(normalMat * normals);
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);
    
    mat3 TBN = transpose(mat3(T, B, N));    
    tangentLightDir = TBN * light.direction;
    tangentCamPos  = TBN * cameraPos;
	tangentFragPos = TBN * fragPos;
}

