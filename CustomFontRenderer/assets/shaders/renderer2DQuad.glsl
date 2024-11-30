// ===========================================================================
#type vertex
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in int a_TexIndex;
layout(location = 4) in float a_TilingFactor;
		
uniform mat4 u_ViewProjectionMatrix;

struct VertexOutput
{
	vec4 color;
	vec2 texCoord;
	float tilingFactor;
};

layout (location = 0) out VertexOutput v_Output;
layout (location = 3) out flat int v_TexIndex;

void main()
{
	v_Output.color = a_Color;
	v_Output.texCoord = a_TexCoord;
	v_Output.tilingFactor = a_TilingFactor;
	v_TexIndex = a_TexIndex;

	gl_Position = u_ViewProjectionMatrix * vec4(a_Position, 1.0);
}

// ===========================================================================
#type fragment
#version 450 core
		
layout(location = 0) out vec4 o_Color;

struct VertexInput
{
	vec4 color;
	vec2 texCoord;
	float tilingFactor;
};

layout(location = 0) in VertexInput v_Input;
layout(location = 3) in flat int v_TexIndex;

layout(binding = 0) uniform sampler2D u_Texture[32];

void main()
{
	//vec4 outColor = vec4(v_TexCoord, 0.0, 1.0); // debugging uv coords (texCoord)
	//vec4 outColor = v_Input.color; // pure color output
	vec4 outColor = texture(u_Texture[v_TexIndex], v_Input.texCoord * v_Input.tilingFactor) * v_Input.color;

	if (outColor.a == 0.0) discard;
	else o_Color = outColor;
}
// ===========================================================================