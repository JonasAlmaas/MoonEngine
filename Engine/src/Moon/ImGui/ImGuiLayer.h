#pragma once

#include "Moon/Core/Layer/Layer.h"


namespace Moon {

	class MOON_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach();
		virtual void OnDetach();
		virtual void OnUpdate();
		virtual void OnEvent(Event& e);

	private:

	};

}
