#version 410 core

in vec3 normal;
in vec4 fragPosEye;
in vec4 fragPosLightSpace;
//in vec4 fragPosLightSpaceLamp;
//in vec4 fragPosLightSpaceLamp2;
in vec2 fragTexCoords;

out vec4 fColor;

//lighting
uniform	mat3 normalMatrix;
uniform mat3 lightDirMatrix;
uniform	vec3 lightColor;
uniform	vec3 lightColor2;
uniform	vec3 lightDir;
uniform	vec3 lightPosLamp1;
uniform	vec3 lightPosLamp2;
uniform	vec3 lightPosLamp3;
uniform	vec3 lightPosLamp4;
uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;
uniform sampler2D shadowMap;
uniform sampler2D shadowMapLamp;
uniform float computeFogEnable;

uniform	vec3 lightSpotLamp1;
uniform	vec3 lightSpotLamp2;
uniform	vec3 lightSpotLamp3;
uniform	vec3 lightSpotLamp4;

vec3 ambient;
float ambientStrength = 0.2f;
vec3 diffuse;
vec3 specular;
float specularStrength = 0.5f;
float shininess = 32.0f;


vec3 ambientLamp;
vec3 diffuseLamp;
vec3 specularLamp;

float ambientStrengthLamp = 0.0f;
float specularStrengthLamp = 0.0f;
float shininessLamp = 0.0f;

vec3 o;

float constant = 1.0f;
float linear = 0.0045f;
float quadratic = 0.0075f;

uniform mat4 view;



float cutOff = 0.91f;
float outerCutOff = 0.82f;

float ambientStrengthSpotLamp = 0.02f;
float specularStrengthSpotLamp = 32.0f;
float shininessSpotLamp = 64.0f;



float computeShadow(vec4 fragPosLightSpace, sampler2D shadowMap)
{	
	// perform perspective divide
    vec3 normalizedCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    if(normalizedCoords.z > 1.0f)
        return 0.0f;
    // Transform to [0,1] range
    normalizedCoords = normalizedCoords * 0.5f + 0.5f;
    // Get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, normalizedCoords.xy).r;    
    // Get depth of current fragment from light's perspective
    float currentDepth = normalizedCoords.z;
    // Check whether current frag pos is in shadow
    float bias = 0.005f;
    float shadow = currentDepth - bias> closestDepth  ? 1.0f : 0.0f;

    return shadow;	
}



vec3 computeLightComponents()
{		
	vec3 cameraPosEye = vec3(0.0f);//in eye coordinates, the viewer is situated at the origin
	
	//transform normal
	vec3 normalEye = normalize(normalMatrix * normal);	
	
	//compute light direction
	vec3 lightDirN = normalize(lightDirMatrix * lightDir);	

	//compute view direction 
	vec3 viewDirN = normalize(cameraPosEye - fragPosEye.xyz);
	
	//compute half vector
	vec3 halfVector = normalize(lightDirN + viewDirN);
	
	//compute ambient light
	ambient = ambientStrength * lightColor;
	
	//compute diffuse light
	diffuse = max(dot(normalEye, lightDirN), 0.0f) * lightColor;
	
	//compute specular light
	float specCoeff = pow(max(dot(halfVector, normalEye), 0.0f), shininess);
	specular = specularStrength * specCoeff * lightColor;
	
	//modulate with diffuse map
	ambient *= vec3(texture(diffuseTexture, fragTexCoords));
	diffuse *= vec3(texture(diffuseTexture, fragTexCoords));
	//modulate woth specular map
	specular *= vec3(texture(specularTexture, fragTexCoords));
	
	//return ambient + diffuse + specular;
	
	float shadow = computeShadow(fragPosLightSpace, shadowMap);
	return min((ambient + (1.0f - shadow)*diffuse) + (1.0f - shadow)*specular, 1.0f);
	 
}

vec3 computeLightPointComponentsLamp(vec4 lightPosEye)
{		
	vec3 cameraPosEye = vec3(0.0f);//in eye coordinates, the viewer is situated at the origin
	
	//transform normal
	vec3 normalEye = normalize(normalMatrix * normal);	
	
	//vec3 lightPosEye = normalize(normalMatrix * lightDirLamp);
	
	
	vec3 lightDirN = normalize(lightPosEye.xyz - fragPosEye.xyz);	

	//compute view direction 
	vec3 viewDirN = normalize(cameraPosEye - fragPosEye.xyz);
	
	//compute half vector
	vec3 halfVector = normalize(lightDirN + viewDirN);
		
		
	//compute distance to light
	float dist = length(lightPosEye.xyz - fragPosEye.xyz);
	//compute attenuation
	float att = 1.0f / (constant + linear * dist + quadratic * (dist * dist));
	
	//compute ambient light
	ambientLamp = att * ambientStrengthLamp * lightColor2;
	
	//compute diffuse light
	diffuseLamp = att * max(dot(normalEye, lightDirN), 0.0f) * lightColor2;
	
	//compute specular light
	float specCoeff = pow(max(dot(halfVector, normalEye), 0.0f), shininessLamp);
	specularLamp = att * specularStrengthLamp * specCoeff * lightColor2;
	
	//modulate with diffuse map
	ambientLamp *= vec3(texture(diffuseTexture, fragTexCoords));
	diffuseLamp *= vec3(texture(diffuseTexture, fragTexCoords));
	//modulate woth specular map
	specularLamp *= vec3(texture(specularTexture, fragTexCoords));
	
	return (ambientLamp + diffuseLamp + specularLamp);
}

vec3 computeLightSpotComponentsLamp(vec4 lightPosEye, vec3 lightDirection)
{		
	vec3 cameraPosEye = vec3(0.0f);//in eye coordinates, the viewer is situated at the origin
	
	//transform normal
	vec3 normalEye = normalize(normalMatrix * normal);	
	
	//vec3 lightPosEye = normalize(normalMatrix * lightDirLamp);
	
	
	vec3 lightDirN = normalize(lightPosEye.xyz - fragPosEye.xyz);	

	//compute view direction 
	vec3 viewDirN = normalize(cameraPosEye - fragPosEye.xyz);
	
	//compute half vector
	vec3 halfVector = normalize(lightDirN + viewDirN);
		
		
	//compute distance to light
	float dist = length(lightPosEye.xyz - fragPosEye.xyz);
	//compute attenuation
	float att = 1.0f / (constant + linear * dist + quadratic * (dist * dist));
	
	// spotlight intensity
    float theta = dot(lightDir, normalize(-lightDirection)); 
    float epsilon = cutOff - outerCutOff;
    float intensity = clamp((theta - outerCutOff) / epsilon, 0.0, 1.0);
	
	//compute ambient light
	ambientLamp =  att * ambientStrengthSpotLamp * lightColor;
	
	//compute diffuse light
	diffuseLamp = intensity *att * max(dot(normalEye, lightDirN), 0.0f) * lightColor;
	
	//compute specular light
	float specCoeff = pow(max(dot(halfVector, normalEye), 0.0f), shininessSpotLamp);
	specularLamp = intensity * att * specularStrengthSpotLamp * specCoeff * lightColor;
	
	//modulate with diffuse map
	ambientLamp *= vec3(texture(diffuseTexture, fragTexCoords));
	diffuseLamp *= vec3(texture(diffuseTexture, fragTexCoords));
	//modulate woth specular map
	specularLamp *= vec3(texture(specularTexture, fragTexCoords));
	
	return (ambientLamp + diffuseLamp + specularLamp);
	//return (ambientLamp  + specularLamp);

}

float computeFog()
{
	float fogDensity = 0.20f;
	float fragmentDistance = length(fragPosEye);
	float fogFactor = exp(-pow(fragmentDistance * fogDensity, 2));
	return clamp(fogFactor, 0.0f, 1.0f);
}


void main() 
{
	
	//float shadow = computeShadow(fragPosLightSpace, shadowMap);
	//float shadowLamp = computeShadow(fragPosLightSpaceLamp, shadowMapLamp);
	
	//modulate with shadow
	//vec3 color = min((ambient + (1.0f - shadow)*diffuse) + (1.0f - shadow)*specular, 1.0f);
	
	//modulate with shadow
	//vec3 colorLamp = min((ambientLamp + (1.0f - shadowLamp)*diffuseLamp) + (1.0f - shadowLamp)*specularLamp, 1.0f);
    
	/*vec3 color = min(computeLightComponents() + computeLightComponentsLamp(), 1.0f);
	
	if(shadow != 0 && shadowLamp != 0)
	{
		color = mix(color, vec3(0.0f, 0.0f, 0.0f), 0.9f);
	}
	else if(shadow != 0 || shadowLamp != 0)
	{
		color = mix(color, vec3(0.0f, 0.0f, 0.0f), 0.6f);
	}*/
	
	
	
	vec4 lightPosEye1 = view * vec4(lightPosLamp1, 1.0f);
	vec4 lightPosEye2 = view * vec4(lightPosLamp2, 1.0f);
	vec4 lightPosEye3 = view * vec4(lightPosLamp3, 1.0f);
	vec4 lightPosEye4 = view * vec4(lightPosLamp4, 1.0f);
	
	vec3 result = computeLightComponents();
	result += computeLightPointComponentsLamp(lightPosEye1);
	result += computeLightPointComponentsLamp(lightPosEye2);
	result += computeLightPointComponentsLamp(lightPosEye3);
	result += computeLightPointComponentsLamp(lightPosEye4);
	
	
	
	result +=computeLightSpotComponentsLamp(lightPosEye1,lightSpotLamp1);
	result +=computeLightSpotComponentsLamp(lightPosEye2,lightSpotLamp2);
	result +=computeLightSpotComponentsLamp(lightPosEye3,lightSpotLamp3);
	result +=computeLightSpotComponentsLamp(lightPosEye4,lightSpotLamp4);
	
	
	
    //fColor = vec4(color, 1.0f);
    //fColor = vec4(o, 1.0f);
	
	//vec4 fColorTemp = vec4(color, 1.0f);
	
	vec4 fColorTemp = vec4(result, 1.0f);
	
	if(computeFogEnable != 0.0f)
	{
		float fogFactor = computeFog();
		vec4 fogColor = vec4(0.5f, 0.5f, 0.5f, 1.0f);
		fColor = mix(fogColor, fColorTemp , fogFactor);
	}
	else 
	{
		fColor = fColorTemp ;
	}
	
}
