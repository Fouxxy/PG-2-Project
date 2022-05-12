#version 460 core


// vertex attributes
layout ( location = 0 ) in vec3 in_position_ms;
layout ( location = 1 ) in vec2 aTexCoord;
layout ( location = 2 ) in vec3 aNormal;
layout ( location = 3 ) in vec3 aTangent;
layout ( location = 4 ) in vec3 aColor;

out vec3 position_ws;
out vec3 vertex_normal;
out vec2 texCoords;

// uniform variables
uniform mat4 P; // Model View Projection
uniform mat4 model;




void main( void )
{

	position_ws = vec3(model * vec4(in_position_ms, 1.0));
	vertex_normal = mat3(transpose(inverse(model))) * aNormal;
	texCoords = aTexCoord;
	gl_Position = P * vec4(in_position_ms, 1.0f); // model-space -> clip-space

	
}
