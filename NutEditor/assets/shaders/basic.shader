#type vertex
#version 450 core

layout(location = 0) in vec3 a_Position;

layout(set = 0, binding = 0) uniform CameraBuffer
{
	mat4 view;
	mat4 projection;
} u_CameraBuffer;

void main()
{
	gl_Position = vec4(a_Position, 1.0);
}


#type fragment
#version 450 core

layout(location = 0) out vec4 o_Color;


layout(push_constant) uniform Constants
{
	vec4 color;
	vec3 lightDirection;
} u_Constants;


void main()
{
	vec3 lDir = u_Constants.lightDirection;

	o_Color = u_Constants.color - vec4(lDir, 1.0);
}
