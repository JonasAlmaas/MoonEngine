#pragma once


namespace Moon {

	class Entity
	{
	public:
		Entity() = default;
		Entity(entt::registry* registry);
		Entity(entt::registry* registry, entt::entity entityHandle);
		Entity(const Entity& other) = default;
		
		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			ME_CORE_ASSERT(!HasComponent<T>(), "Entity already has this component!");
			return m_Registry->emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
		}

		template<typename T>
		void RemoveComponent()
		{
			ME_CORE_ASSERT(HasComponent<T>(), "Entity does not have this component!");
			m_Registry->remove<T>(m_EntityHandle);
		}

		template<typename T>
		T& GetComponent()
		{
			ME_CORE_ASSERT(HasComponent<T>(), "Entity does not have this component!");
			return m_Registry->get<T>(m_EntityHandle);
		}

		template<typename T>
		bool HasComponent()
		{
			return m_Registry->all_of<T>(m_EntityHandle);
		}

		operator bool() const { return m_EntityHandle != entt::null; };

	private:
		entt::registry* m_Registry = nullptr;
		entt::entity m_EntityHandle;

	};

}
