#include "mepch.h"
#include "Moon/Core/Util/Random.h"


namespace Moon {

	std::mt19937 Random::s_RandomEngine;

	std::uniform_int_distribution<std::mt19937::result_type> Random::s_DistributionFloat;

	std::uniform_int_distribution<int> Random::s_IntDistribution;

	std::uniform_int_distribution<uint16_t> Random::s_UIntDistribution16;
	std::uniform_int_distribution<uint32_t> Random::s_UIntDistribution32;
	std::uniform_int_distribution<uint64_t> Random::s_UIntDistribution64;

	std::bernoulli_distribution Random::s_BoolDistribution;

	void Random::Init()
	{
		s_RandomEngine.seed(std::random_device()());
	}

	int Random::Int()
	{
		return s_IntDistribution(s_RandomEngine);
	}

	uint16_t Random::UInt16()
	{
		return s_UIntDistribution16(s_RandomEngine);
	}

	uint32_t Random::UInt32()
	{
		return s_UIntDistribution32(s_RandomEngine);
	}

	uint64_t Random::UInt64()
	{
		return s_UIntDistribution64(s_RandomEngine);
	}

	uint64_t Random::UInt64(uint64_t min, uint64_t max)
	{
		return min + (s_UIntDistribution64(s_RandomEngine) % (max - min + 1));
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
