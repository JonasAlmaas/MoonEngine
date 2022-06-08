#pragma once

#include "Moon/Core/Layer/Layer.h"

#include "Moon/Events/ApplicationEvent.h"
#include "Moon/Events/KeyEvent.h"
#include "Moon/Events/MouseEvent.h"


namespace Moon {

	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;

		void Begin();
		void End();

	private:
		void SetDarkThemeColors();

	};

}
