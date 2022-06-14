#pragma once

#include "Moon/Core/Layer/Layer.h"

#include "Moon/Core/Event/ApplicationEvent.h"
#include "Moon/Core/Event/KeyEvent.h"
#include "Moon/Core/Event/MouseEvent.h"


namespace Moon {

	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		void Begin();
		void End();

	private:
		void SetStyle();

	};

}