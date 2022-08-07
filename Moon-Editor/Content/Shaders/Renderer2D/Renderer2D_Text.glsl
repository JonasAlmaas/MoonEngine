// Text 2D shader

#type vertex
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_UV;
layout(location = 3) in float a_TextureIndex;

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

void main()
{
	Output.Color = a_Color;
	Output.UV = a_UV;

	v_TextureIndex = a_TextureIndex;

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

layout (binding = 0) uniform sampler2D u_FontAtlases[32];

float median(float r, float g, float b)
{
    return max(min(r, g), min(max(r, g), b));
}

float ScreenPxRange()
{
	float pxRange = 2.0f;
    vec2 unitRange = vec2(pxRange) / vec2(textureSize(u_FontAtlases[int(v_TextureIndex)], 0));
    vec2 screenTexSize = vec2(1.0) / fwidth(Input.UV);
    return max(0.5*dot(unitRange, screenTexSize), 1.0);
}

void main()
{
	vec4 bgColor = vec4(Input.Color.rgb, 0.0);
	vec4 fgColor = Input.Color;

	vec3 msd = texture(u_FontAtlases[int(v_TextureIndex)], Input.UV).rgb;
    float sd = median(msd.r, msd.g, msd.b);
    float screenPxDistance = ScreenPxRange() * (sd - 0.5);
    float opacity = clamp(screenPxDistance + 0.5, 0.0, 1.0);

	o_Color = mix(bgColor, fgColor, opacity);
	o_EntityID = -1;
}
