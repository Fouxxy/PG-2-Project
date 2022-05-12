#version 460 core

// vertex attributes
layout ( location = 0 ) in vec3 in_position_ms;
layout ( location = 1 ) in vec2 aTexCoord;
layout ( location = 2 ) in vec3 aNormal;
layout ( location = 3 ) in vec3 aTangent;
layout ( location = 4 ) in vec3 aColor;


//out vec2 texCoord;
out vec4 color;

// uniform variables
uniform mat4 P; // Model View Projection

// vezme vsechny vertexy a vynasobi je matici projekce
void main( void )
{
	gl_Position = P * vec4(in_position_ms, 1.0f); // model-space -> clip-space
	//texCoord = vec2(aTexCoord.x, 1.0 - aTexCoord.y);
	color = vec4(aNormal * 0.5 + 0.5, 1.0f);
}
