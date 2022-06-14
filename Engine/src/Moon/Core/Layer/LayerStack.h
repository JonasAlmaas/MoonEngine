#pragma once

#include "Moon/Core/Base.h"
#include "Moon/Core/Layer/Layer.h"


namespace Moon {

	class LayerStack
	{
	public:
		LayerStack();
		~LayerStack() = default;

		void PushLayer(Ref<Layer> layer);
		void PushOverlay(Ref<Layer> overlay);
		bool PopLayer(Ref<Layer> layer);
		bool PopOverlay(Ref<Layer> overlay);

		void OnUpdate(Timestep ts);
		void OnImGuiRender();

		std::vector<Ref<Layer>>::iterator begin() { return m_Layers.begin(); }
		std::vector<Ref<Layer>>::iterator end() { return m_Layers.end(); }

	private:
		std::vector<Ref<Layer>> m_Layers;
		unsigned int m_LayerInsertIndex = 0;

	};

}
