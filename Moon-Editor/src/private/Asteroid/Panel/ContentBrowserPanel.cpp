#include "aopch.h"
#include "Asteroid/Panel/ContentBrowserPanel.h"


namespace Asteroid {

	// Once we have projects, change this
	extern const std::filesystem::path g_ContentPath = "Content";

	ContentBrowserPanel::ContentBrowserPanel()
		: m_CurrentDirectory(g_ContentPath)
	{
	}

	void ContentBrowserPanel::OnAttach()
	{
		m_IconFolder = Texture2D::Create("Resources/Editor/ContentBrowser/Folder.png");
		m_IconFile = Texture2D::Create("Resources/Editor/ContentBrowser/File.png");
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

		if (m_CurrentDirectory != g_ContentPath)
		{
			if (ImGui::Button("<-"))
			{
				m_CurrentDirectory = m_CurrentDirectory.parent_path();
			}
		}

		static float padding = 16.0f;
		static float thumbnailSize = 98.0f;
		float cellSize = thumbnailSize + padding;

		float panelWidth = ImGui::GetContentRegionAvail().x;
		int columnCount = (int)(panelWidth / cellSize);
		if (columnCount < 1)
			columnCount = 1;

		ImGui::Columns(columnCount, 0, false);

		for (auto& directoryEntry : std::filesystem::directory_iterator(m_CurrentDirectory))
		{
			const auto& path = directoryEntry.path();
			std::string filenameString = path.filename().string();

			ImGui::PushID(filenameString.c_str());
			Ref<Texture2D> icon = directoryEntry.is_directory() ? m_IconFolder : m_IconFile;

			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0,0,0,0));
			ImGui::ImageButton((ImTextureID)(uint64_t)icon->GetRendererID(), {thumbnailSize, thumbnailSize}, {0, 1}, {1, 0});
			ImGui::PopStyleColor();
			ImGui::PopStyleVar();

			auto relativePath = std::filesystem::relative(path, g_ContentPath);

			// TODO: Figure out how to show a tooltip when dragging
			if (relativePath.extension().string() == ".mmap")
			{
				if (ImGui::BeginDragDropSource())
				{
					const wchar_t* payload = relativePath.c_str();
					ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM_MAP", payload, (wcslen(payload) + 1)* sizeof(wchar_t));
					ImGui::EndDragDropSource();
				}
			}
			// For now texture can only be pngs
			// TODO: Texture should be a custom file format, "masset" maybe?
			else if (relativePath.extension().string() == ".png")
			{
				if (ImGui::BeginDragDropSource())
				{
					const wchar_t* payload = relativePath.c_str();
					ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM_TEXTURE", payload, (wcslen(payload) + 1) * sizeof(wchar_t));
					ImGui::EndDragDropSource();
				}
			}

			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				if (directoryEntry.is_directory())
				{
					m_CurrentDirectory /= path.filename();
				}
				else
				{
				}
			}
			ImGui::TextWrapped(filenameString.c_str());

			ImGui::NextColumn();

			ImGui::PopID();
		}

		ImGui::Columns(1);

		// TODO: Create a gear icon in the top right. It should have a dropdown where you can change these values
		//ImGui::SliderFloat("Thumbnail Size", &thumbnailSize, 16, 512);
		//ImGui::SliderFloat("Padding", &padding, 0, 32);

		// TODO: status bar

		ImGui::End();
	}

	// ---- Event Handling ----

	void ContentBrowserPanel::OnEvent(Event& e)
	{
	}

}
