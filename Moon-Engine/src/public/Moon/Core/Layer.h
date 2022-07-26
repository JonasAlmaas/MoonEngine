#pragma once

#include "Moon/Core/Event.h"
#include "Moon/Core/Type/Timestep.h"


namespace Moon {

	class Layer
	{
	public:
		Layer(const std::string& name = "Layer")
			: m_DebugName(name) {}

		virtual ~Layer() = default;

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(Timestep ts) {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& e) {}

		inline const std::string& GetName() const { return m_DebugName; }

	private:
		std::string m_DebugName;
	};

}
