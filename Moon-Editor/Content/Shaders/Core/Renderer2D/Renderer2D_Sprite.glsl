// Basic 2D Shader

#type vertex
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_UV;
layout(location = 3) in float a_TextureIndex;
layout(location = 4) in int a_EntityID;

layout(std140, binding = 0) uniform Camera
{
	mat4 u_ViewProjection;
};

struct VertexOutput
{
	vec4 Color;
	vec2 UV;
};

layout (location = 0) out VertexOutput Output;
layout (location = 3) out flat float v_TextureIndex;
layout (location = 4) out flat int v_EntityID;

void main()
{
	Output.Color = a_Color;
	Output.UV = a_UV;

	v_TextureIndex = a_TextureIndex;
	v_EntityID = a_EntityID;

	gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
}


#type pixel
#version 450 core

layout(location = 0) out vec4 o_Color;
layout(location = 1) out int o_EntityID;

struct VertexOutput
{
	vec4 Color;
	vec2 UV;
};

layout (location = 0) in VertexOutput Input;
layout (location = 3) in flat float v_TextureIndex;
layout (location = 4) in flat int v_EntityID;

layout (binding = 0) uniform sampler2D u_Textures[32];

float Lerp(float x, float y, float a)
{
	return x + ((y - x) * a);
}

vec3 Lerp3(vec3 v1, vec3 v2, float a)
{
	return vec3(Lerp(v1.x, v2.x, a), Lerp(v1.y, v2.y, a), Lerp(v1.z, v2.z, a));
}

void main()
{
	vec4 color = texture(u_Textures[int(v_TextureIndex)], Input.UV);
	o_Color = vec4(Lerp3(color.xyz, Input.Color.xyz, Input.Color.a), color.a);

	if (o_Color.a == 0.0)
		discard;

	o_EntityID = v_EntityID;
}
