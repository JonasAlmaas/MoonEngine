#include "mepch.h"
#include "Moon/Scene/Entity/Entity.h"


namespace Moon {

	Entity::Entity(entt::registry* registry)
		: m_Registry(registry), m_EntityHandle(m_Registry->create())
	{
	}

	Entity::Entity(entt::registry* registry, entt::entity entityHandle)
		: m_Registry(registry), m_EntityHandle(entityHandle)
	{
	}

}
