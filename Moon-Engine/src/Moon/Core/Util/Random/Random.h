#pragma once

#include <random>


namespace Moon {

	class Random
	{
	public:
		static void Init();

		/*
		 * @return A random unsignend 16 bit integer.
		 */
		static uint16_t UInt16();

		/*
		 * @return A random unsignend 32 bit integer.
		 */
		static uint32_t UInt32();

		/*
		 * @return A random unsignend 64 bit integer.
		 */
		static uint64_t UInt();

		/*
		 * Returns a random unsignend 64 bit integer between min and max. Inclusive.
		 * 
		 * @param Minimum value the integer can be.
		 * @param Maximum value the integer can be.
		 */
		static uint64_t UInt(uint32_t min, uint32_t max);

		/*
		 * @brief Returns a random float between 0 and 1. Inclusive.
		 */
		static float Float();

		/*
		 * @brief Returns a random boolean (either true or false).
		 */
		static bool Bool();

	private:
		static std::mt19937 s_RandomEngine;
		static std::uniform_int_distribution<uint16_t> s_Distribution16;
		static std::uniform_int_distribution<uint32_t> s_Distribution32;
		static std::uniform_int_distribution<uint64_t> s_Distribution64;
		static std::uniform_int_distribution<std::mt19937::result_type> s_DistributionFloat;
		static std::bernoulli_distribution s_BoolDistribution;

	};

}
