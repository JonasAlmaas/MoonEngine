#pragma once

#include "Moon/Core/Renderer/RendererAPI/RendererAPI.h"


namespace Moon {

	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		virtual void SetClearColor(const Color& color) override;
		virtual void Clear() override;

		virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) override;

	};

}
