#pragma once

#include "Moon/Core/Layer/Layer.h"


namespace Moon {

	class LayerStack
	{
	public:
		LayerStack();
		~LayerStack() = default;

		/*
		 * Adds a layer to the layer stack.
		 * 
		 * @param Layer to add.
		 */
		void PushLayer(Ref<Layer> layer);

		/*
		 * Adds an overlay to the layer stack.
		 *
		 * @param Overlay layer to add.
		 */
		void PushOverlay(Ref<Layer> overlay);

		/*
		 * Removes a layer from the layer stack.
		 * 
		 * @param Layer to remove.
		 * 
		 * @return Whether the layer was found and removed.
		 */
		bool PopLayer(Ref<Layer> layer);
		
		/*
		 * Removes an overlay from the layer stack.
		 *
		 * @param Overlay layer to remove.
		 *
		 * @return Whether the overlay was found and removed.
		 */
		bool PopOverlay(Ref<Layer> overlay);

		void OnUpdate(Timestep ts);
		void OnImGuiRender();
		void OnEvent(Event& e);

		std::vector<Ref<Layer>>::iterator begin() { return m_Layers.begin(); }
		std::vector<Ref<Layer>>::iterator end() { return m_Layers.end(); }

	private:
		std::vector<Ref<Layer>> m_Layers;
		unsigned int m_LayerInsertIndex = 0;

	};

}
