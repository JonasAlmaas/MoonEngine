#pragma once

#include "Moon/Core/Layer/Layer.h"

#include "Moon/Core/Events/ApplicationEvent.h"
#include "Moon/Core/Events/KeyEvent.h"
#include "Moon/Core/Events/MouseEvent.h"


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
