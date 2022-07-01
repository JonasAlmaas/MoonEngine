#pragma once

#include <random>


namespace Moon {

	class Random
	{
	public:
		static void Init();

		/**
		 * @brief Returns a random unsignend 32 bit integer between min and max. Inclusive.
		 */
		static uint32_t UInt();
		
		/*
		 * Returns a random unsignend 32 bit integer between min and max. Inclusive.
		 * 
		 * @param Minimum value the integer can be.
		 * @param Maximum value the integer can be.
		 */
		static uint32_t UInt(uint32_t min, uint32_t max);

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
		static std::uniform_int_distribution<uint32_t> s_Distribution;
		static std::bernoulli_distribution s_BoolDistribution;

	};

}
