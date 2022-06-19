#pragma once


namespace Moon {

	class Random
	{
	public:
		static void Init();

		/*
		 * Returns a random integer between min and max. Inclusive.
		 * 
		 * @param Minimum value the integer can be.
		 * @param Maximum value the integer can be.
		 */
		static int Int(int min = 0, int max = std::numeric_limits<uint32_t>::max());

		/*
		 * @brief Returns a random float between 0 and 1. Inclusive.
		 */
		static float Float();

		/*
		 * @brief Returns either true or false.
		 */
		static bool Bool();

	private:
		static std::mt19937 s_RandomEngine;
		static std::uniform_int_distribution<uint32_t> s_FloatDistribution;
		static std::bernoulli_distribution s_BoolDistribution;

	};

}
