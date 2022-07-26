#pragma once

using namespace Moon;

#include "Asteroid/Panel.h"


namespace Asteroid {
	
	class RendererPanel : public Panel
	{
	public:
		RendererPanel() = default;
		virtual ~RendererPanel() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate(Timestep ts) override;
		virtual void OnImGuiRender() override;

	private:
		const unsigned char* m_GPUVendor;
		const unsigned char* m_GPUName;
		const unsigned char* m_GPUFirmwareVersion;

		bool m_IsVSync;

		float m_FrameTime;

	};

}
