#pragma once

using namespace Moon;

#include "Asteroid/Panel.h"


namespace Asteroid {

	class ContentBrowserPanel : public Panel
	{
	public:
		ContentBrowserPanel();
		virtual ~ContentBrowserPanel() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate(Timestep ts) override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& e) override;

	private:
		std::filesystem::path m_CurrentDirectory;

		Ref<Texture2D> m_IconFolder;
		Ref<Texture2D> m_IconFile;
	};

}
