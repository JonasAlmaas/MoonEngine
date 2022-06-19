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

		void SetActiveCamera(Entity& camera);

		Entity CreateEntity(const std::string& name = std::string());

	private:
		entt::registry m_Registry;
		Entity* m_ActiveCamera = nullptr;

		uint32_t m_ViewportWidth = 0;
		uint32_t m_ViewportHeight = 0;
	
	};

}
