#include "aopch.h"
#include "Asteroid/Panel/RendererPanel.h"

#include "Asteroid/Panel/UILibrary.h"


namespace Asteroid {

	void RendererPanel::OnAttach()
	{
		m_GPUVendor = RenderCommand::GetGPUVendor();
		m_GPUName = RenderCommand::GetGPUName();
		m_GPUFirmwareVersion = RenderCommand::GetGPUFirmwareVersion();

		m_IsVSync = Application::Get().GetWindow().IsVSync();
	}

	void RendererPanel::OnDetach()
	{

	}

	void RendererPanel::OnUpdate(Timestep ts)
	{
		m_FrameTime = ts;
	}

	void RendererPanel::OnImGuiRender()
	{
		ImGui::Begin("Renderer");

		ImGui::Text("Vendor: %s", m_GPUVendor);
		ImGui::Text("Renderer: %s", m_GPUName);
		ImGui::Text("Version: %s", m_GPUFirmwareVersion);
		ImGui::Separator();
		ImGui::Text("Frame Time: %.2fms", m_FrameTime * 1000.0f);
		ImGui::Text("Fps: %d", (int)(1.0f / m_FrameTime));

		if (UILibrary::Checkbox("VSync", &m_IsVSync))
			Application::Get().GetWindow().SetVSync(m_IsVSync);

		ImGui::End();
	}

}
