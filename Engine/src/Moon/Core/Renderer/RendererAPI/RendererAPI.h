#pragma once

#include "Moon/Core/Renderer/VertexArray/VertexArray.h"


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
		virtual ~RendererAPI() = default;

		virtual void Init() = 0;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;

		virtual void SetClearColor(const Color& color) = 0;
		virtual void Clear() = 0;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0) = 0;

		virtual const unsigned char* GetGPUVendor() = 0;
		virtual const unsigned char* GetGPUName()  = 0;
		virtual const unsigned char* GetGPUFirmwareVersion() = 0;

		inline static API GetAPI() { return s_API; }

	private:
		static API s_API;

	};

}
