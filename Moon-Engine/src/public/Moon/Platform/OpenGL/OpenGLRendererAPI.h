#pragma once

#include "Moon/Core/Renderer/RendererAPI.h"


namespace Moon {

	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		virtual void Init() override;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;

		virtual void SetClearColor(const Color& color) override;
		virtual void Clear() override;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0) override;

		virtual void DrawLines(const Ref<VertexArray>& vertexArray, uint32_t vertexCount) override;
		virtual void SetLineWidth(float width) override;

		virtual void SetDepthTest(bool state) const override;
		virtual void SetMultisample(bool state) const override;

		virtual const unsigned char* GetGPUVendor() override;
		virtual const unsigned char* GetGPUName() override;
		virtual const unsigned char* GetGPUFirmwareVersion() override;

	};

}