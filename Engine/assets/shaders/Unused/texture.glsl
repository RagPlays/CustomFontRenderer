// ===========================================================================
#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;
		
uniform mat4 u_ViewProjectionMatrix;
uniform mat4 u_ModelMatrix;

out vec2 v_TexCoord;

void main()
{
	v_TexCoord = a_TexCoord;
	gl_Position = u_ViewProjectionMatrix * u_ModelMatrix * vec4(a_Position, 1.0);
}

// ===========================================================================
#type fragment
#version 330 core
		
layout(location = 0) out vec4 o_Color;

uniform float u_TilingFactor;
uniform sampler2D u_Texture;
uniform vec4 u_TintColor;

in vec2 v_TexCoord;

void main()
{
	//o_Color = vec4(v_TexCoord, 0.0, 1.0); // debugging uv coords (texCoord)
	o_Color = texture(u_Texture, v_TexCoord * u_TilingFactor) * u_TintColor;
}