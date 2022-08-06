#include "mepch.h"
#include "Moon/Renderer/TextRenderer.h"


namespace Moon {

	struct TextRendererData
	{

	};

	TextRendererData* s_Data;

	void TextRenderer::Init()
	{
		s_Data = new TextRendererData();
	}

	void TextRenderer::Shutdown()
	{
		delete s_Data;
	}

}
