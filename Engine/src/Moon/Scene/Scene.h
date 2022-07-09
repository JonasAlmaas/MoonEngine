#pragma once

#include "Moon/Scene/Entity/Entity.h"

class b2World;


namespace Moon {

	class Scene
	{
	public:
		Scene() = default;
		virtual ~Scene();

		void OnRuntimeStart();
		void OnRuntimeStop();
		void OnRuntimeUpdate(Timestep ts);

		void OnSimulationStart();
		void OnSimulationStop();
		void OnUpdateSimulation(Timestep ts, const glm::mat4& viewProj);

		void OnViewportResize(uint32_t width, uint32_t height);

		Entity CreateEntity(const std::string& name = std::string());
		Entity CreateEntityWithUUID(UUID uuid, const std::string& name = std::string());
		void DuplicateEntity(Entity entity);
		void DestroyEntity(Entity entity);

		void SetActiveCamera(Entity camera = {});
		Entity GetActiveCamera() { return m_ActiveCamera; }

		template<typename... Components>
		auto GetAllEntitiesWith()
		{
			return m_Registry.view<Components...>();
		}

		entt::registry& GetRegistry() { return m_Registry; }

	private:
		void OnPhysics2DStart();
		void OnPhysics2DStop();

		void RenderScene(const glm::mat4& viewProj);

	protected:
		entt::registry m_Registry;

		Entity m_ActiveCamera{};

		uint32_t m_ViewportWidth = 0;
		uint32_t m_ViewportHeight = 0;

	private:
		b2World* m_PhysicsWorld = nullptr;
	
	};

}
