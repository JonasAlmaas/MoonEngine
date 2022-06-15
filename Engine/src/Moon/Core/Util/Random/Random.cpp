#include "mepch.h"
#include "Moon/Core/Util/Random/Random.h"


namespace Moon {

	std::mt19937 Random::s_RandomEngine = std::mt19937();
	std::uniform_int_distribution<uint32_t> Random::s_FloatDistribution = std::uniform_int_distribution<uint32_t>(0, std::numeric_limits<uint32_t>::max());
	std::bernoulli_distribution Random::s_BoolDistribution = std::bernoulli_distribution();

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
		return (float)s_FloatDistribution(s_RandomEngine) / (float)std::numeric_limits<uint32_t>::max();
	}

	bool Random::Bool()
	{
		return s_BoolDistribution(s_RandomEngine);
	}

}
