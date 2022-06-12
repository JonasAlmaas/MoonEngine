#pragma once

#include "Moon/Core/Base.h"
#include "Moon/Core/Renderer/RendererAPI/RendererAPI.h"


namespace Moon {

	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		virtual void Init() override;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;

		virtual void SetClearColor(const Color& color) override;
		virtual void Clear() override;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) override;

	};

}
