#pragma once


namespace Moon {

	class Entity
	{
	private:
		struct EntityHandle
		{
			static constexpr uint32_t Null = std::numeric_limits<uint32_t>::max();

			uint32_t Handle = Null;

			EntityHandle() = default;

			EntityHandle(uint32_t handle)
				: Handle(handle) {}

			EntityHandle(entt::entity handle)
				: Handle((uint32_t)handle) {}

			operator uint32_t () { return Handle; }
			operator entt::entity() { return (entt::entity)Handle; }

			bool operator == (const EntityHandle& other) const { return Handle == other.Handle; }
			bool operator == (const auto& other) const { return Handle == other; }
		};

	public:
		Entity() = default;
		Entity(entt::registry* registry);
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

		operator bool() const { return m_EntityHandle != EntityHandle::Null; };

	private:
		entt::registry* m_Registry = nullptr;
		EntityHandle m_EntityHandle;

	};

}
