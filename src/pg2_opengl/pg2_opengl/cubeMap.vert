#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 projection;
uniform mat4 view;

out vec3 position_ws;

void main()
{
    position_ws = aPos;

	mat4 rotView = mat4(mat3(view));
	vec4 clipPos = projection * rotView * vec4(position_ws, 1.0);

	gl_Position = clipPos.xyww;
}