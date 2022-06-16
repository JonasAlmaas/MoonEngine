// Basic 2D Shader

#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_UV;

out vec4 v_Color;
out vec2 v_UV;

uniform mat4 u_ViewProjection;

void main()
{
	v_Color = a_Color;
	v_UV = a_UV;

	gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
}


#type fragment
#version 330 core

layout(location = 0) out vec4 o_Color;

in vec4 v_Color;
in vec2 v_UV;

uniform sampler2D u_Texture;

void main()
{
	o_Color = texture(u_Texture, v_UV) * v_Color;
}
