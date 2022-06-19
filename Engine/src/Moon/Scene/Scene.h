#pragma once

#include "Moon/Scene/Entity/Entity.h"


namespace Moon {

	class Scene
	{
	public:
		Scene();
		~Scene();

		void OnUpdate(Timestep ts);

		Entity CreateEntity(const std::string& name = std::string());

	private:
		entt::registry m_Registry;

	};

}
