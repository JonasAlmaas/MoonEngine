#pragma once

#include "Moon/Core/Buffer.h"


namespace Moon {

	enum class ImageFormat
	{
		None = 0,
		RED8UN,
		RED8UI,
		RED16UI,
		RED32UI,
		RED32F,
		RG8,
		RG16F,
		RG32F,
		RGB8,
		RGBA8,
		RGBA16F,
		RGBA32F,

		SRGB,

		//DEPTH32FSTENCIL8UINT,
		//DEPTH32F,
		DEPTH24STENCIL8,

		// Defaults
		Depth = DEPTH24STENCIL8,
	};

	enum class ImageUsage
	{
		None = 0,
		Texture,
		Attachment,
		Storage
	};

	enum class TextureWrap
	{
		None = 0,
		Repeat,
		MirroredRepeat,
		Clamp,
		ClampToEdge,
		ClampToBorder,
	};

	enum class TextureFilter
	{
		None = 0,
		Linear,
		Nearest,
		Cubic
	};

	enum class TextureType
	{
		None = 0,
		Texture2D,
		TextureCube
	};

	struct TextureProperties
	{
		//std::string DebugName;
		TextureWrap SamplerWrap = TextureWrap::Repeat;
		TextureFilter SamplerFilter = TextureFilter::Linear;
		//bool GenerateMips = true;
		//bool SRGB = false;
		//bool Storage = false;
	};

}
