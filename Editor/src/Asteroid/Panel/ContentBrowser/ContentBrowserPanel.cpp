#include "aopch.h"
#include "ContentBrowserPanel.h"


namespace Asteroid {

	// Once we have projects, change this
	static const std::filesystem::path s_ContentPath = "Content";

	ContentBrowserPanel::ContentBrowserPanel()
		: m_CurrentDirectory(s_ContentPath)
	{
	}

	void ContentBrowserPanel::OnAttach()
	{
	}

	void ContentBrowserPanel::OnDetach()
	{
	}

	void ContentBrowserPanel::OnUpdate(Timestep ts)
	{
	}

	void ContentBrowserPanel::OnImGuiRender()
	{
		ImGui::Begin("Content Browser");

		if (m_CurrentDirectory != s_ContentPath)
		{
			if (ImGui::Button("<-"))
			{
				m_CurrentDirectory = m_CurrentDirectory.parent_path();
			}
		}

		for (auto& directoryEntry : std::filesystem::directory_iterator(m_CurrentDirectory))
		{
			const auto& path = directoryEntry.path();
			auto relativePath = std::filesystem::relative(path, s_ContentPath);
			std::string filenameString = relativePath.filename().string();

			if (directoryEntry.is_directory())
			{
				if (ImGui::Button(filenameString.c_str()))
				{
					m_CurrentDirectory /= path.filename();
				}
			}
			else
			{
				if (ImGui::Button(filenameString.c_str()))
				{

				}
			}
		}

		ImGui::End();
	}

	// ---- Event Handling ----

	void ContentBrowserPanel::OnEvent(Event& e)
	{
	}

}
