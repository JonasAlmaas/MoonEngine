#pragma once

#include "Moon/Scene/Entity/Entity.h"


namespace Moon {

	class Scene
	{
	public:
		Scene();
		~Scene();

		void OnUpdate(Timestep ts);

		void SetActiveCamera(Entity& camera);

		Entity CreateEntity(const std::string& name = std::string());

	private:
		entt::registry m_Registry;
		Entity* m_ActiveCamera = nullptr;
	
	};

}
