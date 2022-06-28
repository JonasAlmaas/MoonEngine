#pragma once

#include "Moon/Scene/Entity/Entity.h"


namespace Moon {

	class Scene
	{
	public:
		Scene() = default;
		virtual ~Scene() = default;

		void OnUpdateRuntime(Timestep ts);

		void OnViewportResize(uint32_t width, uint32_t height);

		void SetActiveCamera(Entity camera = {});
		Entity GetActiveCamera() { return m_ActiveCamera; }

		Entity CreateEntity(const std::string& name = std::string());
		void DestroyEntity(Entity entity);

		entt::registry& GetRegistry() { return m_Registry; }

	protected:
		entt::registry m_Registry;

	private:
		Entity m_ActiveCamera{};

		uint32_t m_ViewportWidth = 0;
		uint32_t m_ViewportHeight = 0;
	
	};

}
