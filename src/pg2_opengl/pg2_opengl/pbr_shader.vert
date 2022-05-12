#version 460 core


// vertex attributes
layout ( location = 0 ) in vec3 in_position_ms;
layout ( location = 1 ) in vec2 aTexCoord;
layout ( location = 2 ) in vec3 aNormal;
layout ( location = 3 ) in vec3 aTangent;
layout ( location = 4 ) in vec3 aBitangent;
layout ( location = 5 ) in vec3 aColor;

out vec3 position_ws;
out vec3 vertex_normal;
out vec2 texCoords;
out mat3 TBN;

// uniform variables
uniform mat4 P; // Model View Projection
//uniform mat4 model;
uniform int max_level;

void main( void )
{
	
	//tangent, bitangent, normal
	
	//vec3 tangent = normalize(vec3(model * vec4(aTangent, 0.0)));
	//vec3 tangent_normal = normalize(vec3(model * vec4(aNormal, 0.0)));	
	//vec3 bitangent = normalize(vec3(model * vec4(aBitangent, 0.0)));
	
	vec3 tangent = normalize(aTangent);
	vec3 tangent_normal = normalize(aNormal);
	vec3 bitangent = normalize(aBitangent);
	
	mat3 TBN = mat3(tangent,bitangent,tangent_normal);
	//vertex_normal = normalize(mat3(model) * aNormal);
	vertex_normal = aNormal;

	//position_ws = vec3(model * vec4(in_position_ms, 1.0));
	position_ws = in_position_ms;
	// fixes wrong normal direction
	texCoords = vec2(aTexCoord.x, 1 - aTexCoord.y);
	gl_Position = P * vec4(in_position_ms, 1.0f); // model-space -> clip-space	
}
