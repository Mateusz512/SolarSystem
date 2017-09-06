#version 330 core
out vec4 FragColor;

struct PointLight {
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} fs_in;

#define NR_POINT_LIGHTS 4

uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;
uniform sampler2D normalTexture;
uniform sampler2D extraTexture;
uniform samplerCube depthMap;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 atmoColor;

uniform int EnableDiffuseTexture;
uniform int EnableSpecularTexture;
uniform int EnableNormalTexture;
uniform int EnableExtraTexture;

uniform float far_plane;
uniform int isAtmo;

uniform int shadows;

// array of offset direction for sampling
vec3 gridSamplingDisk[20] = vec3[]
(
   vec3(1, 1, 1), vec3(1, -1, 1), vec3(-1, -1, 1), vec3(-1, 1, 1), 
   vec3(1, 1, -1), vec3(1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
   vec3(1, 1, 0), vec3(1, -1, 0), vec3(-1, -1, 0), vec3(-1, 1, 0),
   vec3(1, 0, 1), vec3(-1, 0, 1), vec3(1, 0, -1), vec3(-1, 0, -1),
   vec3(0, 1, 1), vec3(0, -1, 1), vec3(0, -1, -1), vec3(0, 1, -1)
);

// calculates the color when using a point light.
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fs_in.FragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // combine results
    vec3 ambient = light.ambient * vec3(texture(diffuseTexture, fs_in.TexCoords * vec2(1.0, -1.0)));
    vec3 diffuse = light.diffuse * diff * vec3(texture(diffuseTexture, fs_in.TexCoords * vec2(1.0, -1.0)));
	vec3 specular;
	if( EnableSpecularTexture == 1 ){
		specular = light.specular * spec * vec3(texture(specularTexture, fs_in.TexCoords * vec2(1.0, -1.0))); 
	}else{
		specular = light.specular * spec; 
	
	}
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

float ShadowCalculation(vec3 fragPos)
{

    // get vector between fragment position and light position
    vec3 fragToLight = fragPos - lightPos;
    float currentDepth = length(fragToLight);
    float shadow = 0.0;
    float bias = 0.01;
    int samples = 5;
    float viewDistance = length(viewPos - fragPos);
    float diskRadius = (1.0 + (viewDistance / far_plane)) / 150.0;
    for(int i = 0; i < samples; ++i)
    {
        float closestDepth = texture(depthMap, fragToLight + gridSamplingDisk[i] * diskRadius).r;
        closestDepth *= far_plane;   // undo mapping [0;1]
        if(currentDepth - bias > closestDepth)
            shadow += 1.0;
    }
    shadow /= float(samples);
        
    // display closestDepth as debug (to visualize depth cubemap)
    // FragColor = vec4(vec3(closestDepth / far_plane), 1.0);    
        
    return shadow;
}

void main()
{   
	if( EnableDiffuseTexture == 1){
		vec3 color;
		vec4 randomness = vec4(0);
		//FragColor=vec4(color,1);
		vec3 lightDir = normalize(lightPos - fs_in.FragPos);
		vec3 viewDir = normalize(viewPos - fs_in.FragPos);
		vec3 normal = normalize(fs_in.Normal);
		color = texture(diffuseTexture, fs_in.TexCoords * vec2(1.0, -1.0) ).rgb;	
		for(int i = 0; i < NR_POINT_LIGHTS; i++){
			randomness += vec4(CalcPointLight(pointLights[i], normal, fs_in.FragPos, viewDir),1.0);    	
		}
		vec3 lightColor = vec3(0.4);
		vec3 ambient = 0.5 * color; 
		float diff = max(dot(lightDir, normal), 0.0);
		vec3 diffuse = diff * lightColor;
		vec3 reflectDir = reflect(-lightDir, normal);
		float spec = 0.0;
		vec3 specular;
		vec3 halfwayDir = normalize(lightDir + viewDir);  
		spec = pow(max(dot(normal, halfwayDir), 0.0), 128.0);
		if( EnableSpecularTexture == 1 ){
			specular = 3 *  spec * vec3(texture(specularTexture, fs_in.TexCoords * vec2(1.0, -1.0)));
		}else{
			specular = spec * lightColor;
		}
		float shadow;
		vec4 result;
		if(shadows == 1){
			float shadow = ShadowCalculation(fs_in.FragPos); 	
			vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;    
			result = vec4(lighting, 0.5f);
		}else if(shadows == -1){
			result = vec4(color, 1);
		}else{
			//vec3 lighting = (ambient + (diffuse + specular)) * color;  	
			result = vec4(ambient, 0.2f);
		}
		result+=randomness;
		FragColor = vec4(result.xyz,1);	
	}else{
		FragColor = vec4(0.7,0.7,0,0.5);
	}
}