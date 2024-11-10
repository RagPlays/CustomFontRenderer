// ===========================================================================
#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;

uniform mat4 u_ViewProjectionMatrix;
uniform mat4 u_ModelMatrix;

out vec3 v_Position;
out vec4 v_Color;

void main()
{
	v_Position = a_Position;
	v_Color = a_Color;
	gl_Position = u_ViewProjectionMatrix * u_ModelMatrix * vec4(a_Position, 1.0);
}

// ===========================================================================
#type fragment
#version 330 core
		
layout(location = 0) out vec4 o_Color;

in vec3 v_Position;
in vec4 v_Color;

void main()
{
	//o_Color = vec4(v_Position * 0.5 + 0.5, 1.0);
	o_Color = v_Color;
}