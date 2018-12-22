#version 330 core

struct Material{
	sampler2D diffuse;
};

uniform Material material;
in vec2 uv;

void main()
{   
	vec4 texCol = texture(material.diffuse, uv);
	//alpha cutout
	if(texCol.a < 0.5)
       discard;

    gl_FragDepth = gl_FragCoord.z;
}  