#include "mepch.h"
#include "Moon/Core/Renderer/Font.h"

#include "Moon/Core/Util/FileSystem.h"

#include <imstb_truetype.h>
#include <stb_rect_pack.h>


namespace Moon {

	Font::Font(const std::filesystem::path& filepath)
	{
		if (!FileSystem::Exists(filepath))
			return;

		Buffer fontBuffer = FileSystem::ReadBytes(filepath);

		//  Improved 3D API (more shippable):
		//	#include "stb_rect_pack.h"           -- optional, but you really want it
		//	stbtt_PackBegin()
		//  stbtt_PackSetOversampling()          -- for improved quality on small fonts
		//  stbtt_PackFontRanges()               -- pack and renders
		//  stbtt_PackEnd()
		//  stbtt_GetPackedQuad()

		int width = 512;
		int height = 512;
		int stride = 0;

		unsigned char* pixels = new unsigned char[width * height];

		stbtt_pack_context* spc = new stbtt_pack_context();
		int someStateThingIfIWereToGuess = stbtt_PackBegin(spc, pixels, width, height, stride, 1, NULL);

		delete spc;
		delete[] pixels;

		m_Loaded = true;
	}

}
