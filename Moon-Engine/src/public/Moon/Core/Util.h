#pragma once

#include "Moon/Core/Base.h"
#include "Moon/Core/Util/Random.h"


namespace Moon {

	class Util
	{
	public:
		static void Init()
		{
			Random::Init();
		}

	};

	class Time
	{
	public:
		static float Get();

	};

}
