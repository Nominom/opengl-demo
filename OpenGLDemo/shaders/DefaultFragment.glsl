#version 330 core

struct Material{
	sampler2D diffuse;
	sampler2D normalmap;
	vec3 specular;
	vec3 color;
	float shininess;
	float reflectivity;
	bool unlit;
};

struct Light{
	vec3 direction;
	vec3 color;
};

out vec3 color;

in vec2 uv;
in vec3 o_normal;
in vec3 fragPos;
in vec4 fragPosLightSpace;

in vec3 tangentLightDir;
in vec3 tangentCamPos;
in vec3 tangentFragPos;

uniform vec3 cameraPos;
uniform Light light;
uniform Material material;
uniform samplerCube skybox;
uniform sampler2D shadowMap;

float ShadowCalculation(vec4 fragPosLS){
    // perform perspective divide
    vec3 projCoords = fragPosLS.xyz / fragPosLS.w;

    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;

    // Get z from shadow map
    float closestDepth = texture(shadowMap, projCoords.xy).r;

    // get z of frag from light's perspective
    float currentDepth = projCoords.z;

	float bias = max(0.003 * (1.0 - dot(o_normal, light.direction)), 0.002);

    //float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;

	//PCF Filtering
	float shadow = 0.0;
	vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
	for(int x = -1; x <= 1; ++x) {
		for(int y = -1; y <= 1; ++y) {
			float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
			shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
		}    
	}
	shadow /= 9.0;

	if(projCoords.z > 1.0)
        shadow = 0.0;

    return shadow;
}


void main(){
	//Normal mapping
	// obtain normal from normal map in range [0,1]
    vec3 normal = texture(material.normalmap, uv).rgb;
    // transform normal vector to range [-1,1]
	normal = normalize(normal * 2.0 - 1.0); // this normal is in tangent space

	//texture
	vec4 texCol = texture(material.diffuse, uv);
	//vec4 texCol = vec4(normal, 1);

	//alpha cutout
	if(texCol.a < 0.5)
       discard;

	if (material.unlit) {
		color = texCol.rgb;
	} else {
		//blend texture with diffuse color
		vec3 diffuseCol = texCol.rgb * material.color;

		float ambientStrength = 0.4;
		vec3 ambient = light.color * ambientStrength * diffuseCol;

		vec3 lightDir = normalize(-tangentLightDir);
		float diff = max(dot(lightDir, normal), 0.0);
		vec3 diffuse = 0.5 * light.color * diff * diffuseCol;

		vec3 viewDir = normalize(tangentCamPos - tangentFragPos);
		vec3 reflectDir = reflect(-lightDir, normal);
		//vec3 halfwayDir = normalize(lightDir + viewDir);
		float spec = pow(max(dot(normal, reflectDir), 0.0), 32.0);
		vec3 specular =  0.2 * spec * light.color * material.specular;

		float shadow = ShadowCalculation(fragPosLightSpace); 
		
		if(material.reflectivity > 0.01){
			vec3 I = normalize(cameraPos - fragPos);
			vec3 R = reflect(I, o_normal);
			vec3 reflection = texture(skybox, R).rgb;


			color = mix((ambient + diffuse + specular), reflection, material.reflectivity);
		}else{
			color = (ambient + ((1.0 - shadow) * (diffuse + specular)));
		}
	}

}