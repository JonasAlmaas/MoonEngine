#pragma once

#include "Moon/Core/Base.h"
#include "Moon/Core/Renderer/RendererAPI/RendererAPI.h"


namespace Moon {

	class RenderCommand
	{
	public:
		inline static void Init()
		{
			s_RendererAPI->Init();
		}

		inline static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
		{
			s_RendererAPI->SetViewport(x, y, width, height);
		}

		/**
		 * Sets the color used when calling RenderCommand::Clear.
		 *
		 * @param Color, an instance of the Color class.
		 */
		inline static void SetClearColor(const Color& color)
		{
			s_RendererAPI->SetClearColor(color);
		}

		/**
		 * @brief Clears the color buffer.
		 * Set the color by calling RenderCommand::SetClearColor.
		 */
		inline static void Clear()
		{
			s_RendererAPI->Clear();
		}

		inline static void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0)
		{
			s_RendererAPI->DrawIndexed(vertexArray, indexCount);
		}

	private:
		static Scope<RendererAPI> s_RendererAPI;

	};

}