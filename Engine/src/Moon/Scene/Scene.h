#pragma once

#include "Moon/Scene/Entity/Entity.h"


namespace Moon {

	class Scene
	{
	public:
		Scene();
		~Scene();

		void OnUpdate(Timestep ts);

		void OnViewportResize(uint32_t width, uint32_t height);

		void SetActiveCamera(Entity camera = {});
		Entity GetActiveCamera() { return m_ActiveCamera; }

		Entity CreateEntity(const std::string& name = std::string());
		void DestroyEntity(Entity entity);

		Entity GetSelectionContext() { return m_SelectionContext; }
		void SetSelectionContext(Entity entity = {}) { m_SelectionContext = entity; }

		entt::registry& GetRegistry() { return m_Registry; }

	private:
		entt::registry m_Registry;
		Entity m_ActiveCamera{};
		Entity m_SelectionContext{};

		uint32_t m_ViewportWidth = 0;
		uint32_t m_ViewportHeight = 0;
	
	};

}
