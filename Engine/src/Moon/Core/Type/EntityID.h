#pragma once

#include <entt.hpp>


namespace Moon {

	struct EntityID
	{
		uint32_t ID;

		EntityID(uint32_t id)
			: ID(id) {}

		EntityID(entt::entity id)
			: ID((uint32_t)id) {}

		operator uint32_t () { return ID; }
		operator entt::entity() { return (entt::entity)ID; }
	};

}
