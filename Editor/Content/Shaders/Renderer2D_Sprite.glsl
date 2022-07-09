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


#type fragment
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

void main()
{
	vec4 color = Input.Color;

	switch(int(v_TextureIndex))
	{
		case 0:		color *= texture(u_Textures[0], Input.UV); break;
		case 1:		color *= texture(u_Textures[1], Input.UV); break;
		case 2:		color *= texture(u_Textures[2], Input.UV); break;
		case 3:		color *= texture(u_Textures[3], Input.UV); break;
		case 4:		color *= texture(u_Textures[4], Input.UV); break;
		case 5:		color *= texture(u_Textures[5], Input.UV); break;
		case 6:		color *= texture(u_Textures[6], Input.UV); break;
		case 7:		color *= texture(u_Textures[7], Input.UV); break;
		case 8:		color *= texture(u_Textures[8], Input.UV); break;
		case 9:		color *= texture(u_Textures[9], Input.UV); break;
		case 10:	color *= texture(u_Textures[10], Input.UV); break;
		case 11:	color *= texture(u_Textures[11], Input.UV); break;
		case 12:	color *= texture(u_Textures[12], Input.UV); break;
		case 13:	color *= texture(u_Textures[13], Input.UV); break;
		case 14:	color *= texture(u_Textures[14], Input.UV); break;
		case 15:	color *= texture(u_Textures[15], Input.UV); break;
		case 16:	color *= texture(u_Textures[16], Input.UV); break;
		case 17:	color *= texture(u_Textures[17], Input.UV); break;
		case 18:	color *= texture(u_Textures[18], Input.UV); break;
		case 19:	color *= texture(u_Textures[19], Input.UV); break;
		case 20:	color *= texture(u_Textures[20], Input.UV); break;
		case 21:	color *= texture(u_Textures[21], Input.UV); break;
		case 22:	color *= texture(u_Textures[22], Input.UV); break;
		case 23:	color *= texture(u_Textures[23], Input.UV); break;
		case 24:	color *= texture(u_Textures[24], Input.UV); break;
		case 25:	color *= texture(u_Textures[25], Input.UV); break;
		case 26:	color *= texture(u_Textures[26], Input.UV); break;
		case 27:	color *= texture(u_Textures[27], Input.UV); break;
		case 28:	color *= texture(u_Textures[28], Input.UV); break;
		case 29:	color *= texture(u_Textures[29], Input.UV); break;
		case 30:	color *= texture(u_Textures[30], Input.UV); break;
		case 31:	color *= texture(u_Textures[31], Input.UV); break;
	}

	o_Color = color;

	if (texColor.a == 0.0)
		discard;

	o_EntityID = v_EntityID;
}
