#pragma once

using namespace Moon;


namespace Asteroid {

	class MenuBar
	{
	public:
		MenuBar() = default;
		~MenuBar() = default;

		void OnImGuiRender();
		void OnEvent(Event& e);

	private:
		bool OnKeyPressed(KeyPressedEvent& e);

		void NewScene();
		void SaveSceneAs();
		void SaveScene();
		void OpenScene();

	};

}
