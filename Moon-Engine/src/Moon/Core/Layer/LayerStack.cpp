#include "mepch.h"
#include "Moon/Core/Layer/LayerStack.h"


namespace Moon {

	LayerStack::LayerStack()
	{
	}

	LayerStack::~LayerStack()
	{
	}

	void LayerStack::PushLayer(Layer* layer)
	{
		ME_PROFILE_FUNCTION();

		m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex, layer);
		m_LayerInsertIndex++;
	}

	void LayerStack::PushOverlay(Layer* overlay)
	{
		ME_PROFILE_FUNCTION();

		m_Layers.emplace_back(overlay);
	}

	bool LayerStack::PopLayer(Layer* layer)
	{
		ME_PROFILE_FUNCTION();

		auto it = std::find(m_Layers.begin(), m_Layers.end(), layer);
		if (it != m_Layers.end())
		{
			m_Layers.erase(it);
			m_LayerInsertIndex--;
			return true;
		}

		return false;
	}

	bool LayerStack::PopOverlay(Layer* overlay)
	{
		ME_PROFILE_FUNCTION();

		auto it = std::find(m_Layers.begin(), m_Layers.end(), overlay);
		if (it != m_Layers.end())
		{
			m_Layers.erase(it);
			return true;
		}

		return false;
	}

	void LayerStack::OnUpdate(Timestep ts)
	{
		ME_PROFILE_FUNCTION();

		for (Layer* layer : m_Layers)
			layer->OnUpdate(ts);
	}

	void LayerStack::OnImGuiRender()
	{
		ME_PROFILE_FUNCTION();

		for (Layer* layer : m_Layers)
			layer->OnImGuiRender();
	}

	void LayerStack::OnEvent(Event& e)
	{
		ME_PROFILE_FUNCTION();

		// Send events to layers
		for (auto it = m_Layers.end(); it != m_Layers.begin();)
		{
			if (e.Handled)
				break;

			(*--it)->OnEvent(e);
		}
	}

}
