// ===========================================================================
#type vertex
#version 450 core

layout(location = 0) in vec3 a_WorldPosition;
layout(location = 1) in vec3 a_LocalPosition;
layout(location = 2) in vec4 a_Color;
layout(location = 3) in float a_Thickness;
layout(location = 4) in float a_Fade;

uniform mat4 u_ViewProjectionMatrix;

struct VertexOutput
{
	vec3 localPosition;
	vec4 color;
	float thickness;
	float fade;
};

layout (location = 0) out VertexOutput v_Output;

void main()
{
	v_Output.localPosition = a_LocalPosition;
	v_Output.color = a_Color;
	v_Output.thickness = a_Thickness;
	v_Output.fade = a_Fade;

	gl_Position = u_ViewProjectionMatrix * vec4(a_WorldPosition, 1.0);
}

// ===========================================================================
#type fragment
#version 450 core

layout(location = 0) out vec4 o_Color;

struct VertexOutput
{
	vec3 localPosition;
	vec4 color;
	float thickness;
	float fade;
};

layout (location = 0) in VertexOutput v_Input;

void main()
{
	// Calculate distance and fill circle with white
    float distance = 1.0 - length(v_Input.localPosition);
	float circle = smoothstep(0.0, v_Input.fade, distance);
	circle *= smoothstep(v_Input.thickness + v_Input.fade, v_Input.thickness, distance);

	if (circle == 0.0) discard;

	// Set output color
    o_Color = v_Input.color * circle;
}
// ===========================================================================