#version 460 core

// outputs
// layout ( location = 0 ) out vec4 FragColor;
out vec4 FragColor;

// inputs
in vec3 vertex_normal;
in vec3 position_ws;
in vec2 texCoords;

uniform vec3 cameraPosition;

struct Material {
	vec3 diffuse_color;
	vec3 specular_color;
	sampler2D diffuse_texture;
	sampler2D specular_texture;
	float shininess;	
};

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform int useTexture_diffuse = 0;
uniform int useTexture_specular = 0;
uniform Light light;  
uniform Material material;


void main( void )
{	
	// ambient
	vec3 ambientColor = vec3(1.0, 1.0, 1.0);
	if(useTexture_diffuse == 1){
		ambientColor = light.ambient * texture(material.diffuse_texture, texCoords).rgb;
	} else {
		ambientColor = light.ambient * material.diffuse_color;
	}	

	// diffuse 
	vec3 normal = normalize(vertex_normal);
	vec3 lightDirection = normalize(light.position - position_ws);
	float diffuse = max(dot(normal, lightDirection), 0.0);
	
	vec3 diffuseColor = vec3(1.0, 1.0, 1.0);
	if(useTexture_diffuse == 1){
		diffuseColor = light.diffuse * diffuse * texture(material.diffuse_texture, texCoords).rgb;
	} else {
		diffuseColor = light.diffuse * (diffuse * material.diffuse_color);
	}	
	

	// specular 
	vec3 viewDirection = normalize(cameraPosition - position_ws);
	vec3 reflectedDirection = reflect(-lightDirection, normal);
	float specular = pow(max(dot(viewDirection, reflectedDirection), 0.0), material.shininess);
	
	vec3 specularColor = vec3(1.0, 1.0, 1.0);
	if(useTexture_specular == 1){
		specularColor = light.specular * specular * texture(material.specular_texture, texCoords).rgb;
	} else {
		specularColor = light.specular * (specular * material.specular_color);
	}	
	
	// result
	vec3 color = ambientColor + diffuseColor + specularColor;
	FragColor = vec4(color, 1.0);
}
