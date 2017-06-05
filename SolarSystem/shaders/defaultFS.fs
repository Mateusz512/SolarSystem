#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} fs_in;

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

float ShadowCalculation(vec3 fragPos)
{

    // get vector between fragment position and light position
    vec3 fragToLight = fragPos - lightPos;
    float currentDepth = length(fragToLight);
    float shadow = 0.0;
    float bias = 0.15;
    int samples = 20;
    float viewDistance = length(viewPos - fragPos);
    float diskRadius = (1.0 + (viewDistance / far_plane)) / 25.0;
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
	vec3 color;
	if( isAtmo == 1 ){
		if( EnableExtraTexture == 1 ){
			color = texture(extraTexture, 1.0f - fs_in.TexCoords).rgb ;
		}else{			
			color = atmoColor;
		}
	}else{
		color = texture(diffuseTexture, 1.0f - fs_in.TexCoords).rgb;	
	}
	//FragColor=vec4(color,1);
    vec3 normal = normalize(fs_in.Normal);
    vec3 lightColor = vec3(0.4);
    // ambient
    vec3 ambient = 0.5 * color; //DEBUG
    //vec3 ambient = 0.01 * color;
    // diffuse
    vec3 lightDir = normalize(lightPos - fs_in.FragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * lightColor;
    // specular
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = 0.0;
	vec3 specular;
	vec3 halfwayDir = normalize(lightDir + viewDir);  
	spec = pow(max(dot(normal, halfwayDir), 0.0), 8.0);
	if( isAtmo == 0 && EnableSpecularTexture == 1  ){
		//FragColor = vec4(texture(specularTexture, 1.0f - fs_in.TexCoords).rgb, 1.0f);
		specular = 3 *  spec * vec3(texture(specularTexture, 1.0f - fs_in.TexCoords));
	}else{
		//FragColor = vec4(1,0,0, 1.0f);
		specular = spec * lightColor;
	}
    // calculate shadow
	float shadow;
	if(shadows == 1){
		float shadow = ShadowCalculation(fs_in.FragPos); 	
		vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;    
		FragColor = vec4(lighting, 0.5f);
	}else{
		FragColor = vec4(color, 1);
	}                     
}