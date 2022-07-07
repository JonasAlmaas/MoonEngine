#pragma once

#include "Moon/Core/Type/UUID/UUID.h"


namespace Moon {

	struct IDComponent
	{
		UUID ID;

		IDComponent() = default;
		IDComponent(const IDComponent&) = default;
		IDComponent(uint64_t uuid)
			: ID(uuid) {}
	};

}
