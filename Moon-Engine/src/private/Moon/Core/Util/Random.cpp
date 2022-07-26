#include "mepch.h"
#include "Moon/Core/Util/Random.h"


namespace Moon {

	std::mt19937 Random::s_RandomEngine;

	std::uniform_int_distribution<std::mt19937::result_type> Random::s_DistributionFloat;

	std::uniform_int_distribution<uint16_t> Random::s_Distribution16;
	std::uniform_int_distribution<uint32_t> Random::s_Distribution32;
	std::uniform_int_distribution<uint64_t> Random::s_Distribution64;

	std::bernoulli_distribution Random::s_BoolDistribution;

	void Random::Init()
	{
		s_RandomEngine.seed(std::random_device()());
	}

	uint16_t Random::UInt16()
	{
		return s_Distribution16(s_RandomEngine);
	}

	uint32_t Random::UInt32()
	{
		return s_Distribution32(s_RandomEngine);
	}

	uint64_t Random::UInt()
	{
		return s_Distribution64(s_RandomEngine);
	}

	uint64_t Random::UInt(uint32_t min, uint32_t max)
	{
		return min + (s_Distribution64(s_RandomEngine) % (max - min + 1));
	}

	float Random::Float()
	{
		return (float)s_DistributionFloat(s_RandomEngine) / (float)std::numeric_limits<uint32_t>::max();
	}

	bool Random::Bool()
	{
		return s_BoolDistribution(s_RandomEngine);
	}

}
