#include "mepch.h"
#include "Moon/Core/Util/Random/Random.h"


namespace Moon {

	std::mt19937 Random::s_RandomEngine;
	std::uniform_int_distribution<std::mt19937::result_type> Random::s_FloatDistribution;
	std::bernoulli_distribution Random::s_BoolDistribution;

	void Random::Init()
	{
		s_RandomEngine.seed(std::random_device()());
	}

	int Random::Int(int min, int max)
	{
		std::uniform_int_distribution<uint32_t> distrib(min, max);
		return distrib(s_RandomEngine);
	}

	float Random::Float()
	{
		return (float)s_FloatDistribution(s_RandomEngine) / (float)std::numeric_limits<std::mt19937::result_type>::max();
	}

	bool Random::Bool()
	{
		return s_BoolDistribution(s_RandomEngine);
	}

}
