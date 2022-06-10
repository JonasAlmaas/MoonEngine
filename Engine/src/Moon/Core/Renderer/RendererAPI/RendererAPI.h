#pragma once

#include "Moon/Core/Renderer/VertexArray/VertexArray.h"
#include "Moon/Core/Type/Color.h"


namespace Moon {

	class RendererAPI
	{
	public:
		enum class API
		{
			None = 0,
			OpenGL = 1,
		};

	public:
		virtual void SetClearColor(const Color& color) = 0;
		virtual void Clear() = 0;

		virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) = 0;

		inline static API GetAPI() { return s_API; }

	private:
		static API s_API;

	};

}
