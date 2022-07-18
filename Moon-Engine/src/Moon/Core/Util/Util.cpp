#include "mepch.h"
#include "Moon/Core/Util/Util.h"

#include "Moon/Core/Util/Random/Random.h"


namespace Moon {

	void Util::Init()
	{
		ME_PROFILE_FUNCTION();

		Random::Init();
	}

}
