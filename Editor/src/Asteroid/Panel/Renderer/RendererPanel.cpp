#include "aopch.h"
#include "Asteroid/Panel/Renderer/RendererPanel.h"


namespace Asteroid {

	void RendererPanel::OnAttach()
	{
		m_GPUVendor = RenderCommand::GetGPUVendor();
		m_GPUName = RenderCommand::GetGPUName();
		m_GPUFirmwareVersion = RenderCommand::GetGPUFirmwareVersion();
	}

	void RendererPanel::OnDetach()
	{

	}

	void RendererPanel::OnUpdate(Timestep ts)
	{

	}

	void RendererPanel::OnImGuiRender()
	{
		ImGui::Begin("Renderer");

		ImGui::Text("Vendor: %s", m_GPUVendor);
		ImGui::Text("Renderer: %s", m_GPUName);
		ImGui::Text("Version: %s", m_GPUFirmwareVersion);

		float frametime = Application::Get().GetTimestep();
		ImGui::Text("Frame Time: %.2fms", frametime * 1000.0f);
		ImGui::Text("Fps: %d", (int)(1.0f / frametime));

		ImGui::End();
	}

}
