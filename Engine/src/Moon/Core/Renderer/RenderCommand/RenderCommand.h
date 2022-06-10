#pragma once

#include "Moon/Core/Renderer/RendererAPI/RendererAPI.h"


namespace Moon {

	class RenderCommand
	{
	public:
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

		inline static void DrawIndexed(const Ref<VertexArray>& vertexArray)
		{
			s_RendererAPI->DrawIndexed(vertexArray);
		}

	private:
		static RendererAPI* s_RendererAPI;

	};

}