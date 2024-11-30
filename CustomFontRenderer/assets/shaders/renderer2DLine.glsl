// ===========================================================================
#type vertex
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;

uniform mat4 u_ViewProjectionMatrix;

struct VertexOutput
{
	vec4 color;
};

layout (location = 0) out VertexOutput v_Output;

void main()
{
	v_Output.color = a_Color;

	gl_Position = u_ViewProjectionMatrix * vec4(a_Position, 1.0);
}

// ===========================================================================
#type fragment
#version 450 core

layout(location = 0) out vec4 o_Color;

struct VertexOutput
{
	vec4 color;
};

layout (location = 0) in VertexOutput v_Input;

void main()
{
	o_Color = v_Input.color;
}
// ===========================================================================