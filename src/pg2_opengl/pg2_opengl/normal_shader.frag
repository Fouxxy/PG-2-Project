#version 460 core

// outputs
// layout ( location = 0 ) out vec4 FragColor;
out vec4 FragColor;

//in vec2 texCoord;
in vec4 color;

uniform sampler2D texture1;

void main( void )
{	
	//FragColor = vec4( 0.9f, 0.5f, 0.1f, 1.0f );
	//FragColor = texture(texture1, texCoord);
	FragColor = color;
}
