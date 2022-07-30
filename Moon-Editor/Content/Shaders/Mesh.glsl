// Basic Mesh Shader

#type vertex
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TextureUV;

layout(std140, binding = 0) uniform Camera
{
	mat4 u_ViewProjection;
};

struct VertexOutput
{
	vec3 Normal;
	vec2 TextureUV;
};

layout (location = 0) out VertexOutput Output;

void main()
{
	Output.Normal = a_Normal;
	Output.TextureUV = a_TextureUV;

	gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
}


#type pixel
#version 450 core

layout(location = 0) out vec4 o_Color;
layout(location = 1) out int o_EntityID;

struct VertexOutput
{
	vec3 Normal;
	vec2 TextureUV;
};

layout (location = 0) in VertexOutput Input;

layout (binding = 0) uniform sampler2D u_Texture;

void main()
{
	o_Color = texture(u_Texture, Input.TextureUV);
	o_Color.a = 1.0;

	//o_Color = vec4(Input.TextureUV, 0.0, 1.0);
	//o_Color = vec4(Input.Normal, 1.0);

	o_EntityID = -1;
}
