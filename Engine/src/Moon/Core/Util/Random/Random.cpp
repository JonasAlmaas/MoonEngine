#include "mepch.h"
#include "Moon/Core/Util/Random/Random.h"


namespace Moon {

	std::mt19937 Random::s_RandomEngine;
	std::uniform_int_distribution<std::mt19937::result_type> Random::s_Distribution;
	std::bernoulli_distribution Random::s_BoolDistribution;

	void Random::Init()
	{
		s_RandomEngine.seed(std::random_device()());
	}

	uint32_t Random::UInt()
	{
		return s_Distribution(s_RandomEngine);
	}

	uint32_t Random::UInt(uint32_t min, uint32_t max)
	{
		return min + (s_Distribution(s_RandomEngine) % (max - min + 1));
	}

	float Random::Float()
	{
		return (float)s_Distribution(s_RandomEngine) / (float)std::numeric_limits<uint32_t>::max();
	}

	bool Random::Bool()
	{
		return s_BoolDistribution(s_RandomEngine);
	}

}
