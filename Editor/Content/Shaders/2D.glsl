// Basic 2D Shader

#type vertex
#version 450

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_UV;
layout(location = 3) in float a_TextureIndex;
layout(location = 4) in int a_EntityID;

out vec4 v_Color;
out vec2 v_UV;
out flat float v_TextureIndex;
out flat int v_EntityID;

uniform mat4 u_ViewProjection;

void main()
{
	v_Color = a_Color;
	v_UV = a_UV;
	v_TextureIndex = a_TextureIndex;
	v_EntityID = a_EntityID;

	gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
}


#type fragment
#version 450

layout(location = 0) out vec4 o_Color;
layout(location = 1) out int o_EntityID;

in vec4 v_Color;
in vec2 v_UV;
in flat float v_TextureIndex;
in flat int v_EntityID;

uniform sampler2D u_Textures[32];

void main()
{
	vec4 color = v_Color;

	switch(int(v_TextureIndex))
	{
		case 0:		color *= texture(u_Textures[0], v_UV); break;
		case 1:		color *= texture(u_Textures[1], v_UV); break;
		case 2:		color *= texture(u_Textures[2], v_UV); break;
		case 3:		color *= texture(u_Textures[3], v_UV); break;
		case 4:		color *= texture(u_Textures[4], v_UV); break;
		case 5:		color *= texture(u_Textures[5], v_UV); break;
		case 6:		color *= texture(u_Textures[6], v_UV); break;
		case 7:		color *= texture(u_Textures[7], v_UV); break;
		case 8:		color *= texture(u_Textures[8], v_UV); break;
		case 9:		color *= texture(u_Textures[9], v_UV); break;
		case 10:	color *= texture(u_Textures[10], v_UV); break;
		case 11:	color *= texture(u_Textures[11], v_UV); break;
		case 12:	color *= texture(u_Textures[12], v_UV); break;
		case 13:	color *= texture(u_Textures[13], v_UV); break;
		case 14:	color *= texture(u_Textures[14], v_UV); break;
		case 15:	color *= texture(u_Textures[15], v_UV); break;
		case 16:	color *= texture(u_Textures[16], v_UV); break;
		case 17:	color *= texture(u_Textures[17], v_UV); break;
		case 18:	color *= texture(u_Textures[18], v_UV); break;
		case 19:	color *= texture(u_Textures[19], v_UV); break;
		case 20:	color *= texture(u_Textures[20], v_UV); break;
		case 21:	color *= texture(u_Textures[21], v_UV); break;
		case 22:	color *= texture(u_Textures[22], v_UV); break;
		case 23:	color *= texture(u_Textures[23], v_UV); break;
		case 24:	color *= texture(u_Textures[24], v_UV); break;
		case 25:	color *= texture(u_Textures[25], v_UV); break;
		case 26:	color *= texture(u_Textures[26], v_UV); break;
		case 27:	color *= texture(u_Textures[27], v_UV); break;
		case 28:	color *= texture(u_Textures[28], v_UV); break;
		case 29:	color *= texture(u_Textures[29], v_UV); break;
		case 30:	color *= texture(u_Textures[30], v_UV); break;
		case 31:	color *= texture(u_Textures[31], v_UV); break;
	}

	o_Color = color;

	o_EntityID = v_EntityID;
}
