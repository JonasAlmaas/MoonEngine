#pragma once

#include "Moon/Core/Renderer/RendererAPI.h"
#include "Moon/Core/Type/Color.h"


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

		static void DrawLines(const Ref<VertexArray>& vertexArray, uint32_t vertexCount)
		{
			s_RendererAPI->DrawLines(vertexArray, vertexCount);
		}

		static void SetLineWidth(float width)
		{
			s_RendererAPI->SetLineWidth(width);
		}

		static void SetDepthTest(bool state)
		{
			s_RendererAPI->SetDepthTest(state);
		}

		static void SetMultisample(bool state)
		{
			s_RendererAPI->SetMultisample(state);
		}

		inline static const unsigned char* GetGPUVendor()
		{
			return s_RendererAPI->GetGPUVendor();
		}

		inline static const unsigned char* GetGPUName()
		{
			return s_RendererAPI->GetGPUName();
		}

		inline static const unsigned char* GetGPUFirmwareVersion()
		{
			return s_RendererAPI->GetGPUFirmwareVersion();
		}

	private:
		static Scope<RendererAPI> s_RendererAPI;

	};

}
