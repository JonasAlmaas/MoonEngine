#pragma once


namespace Moon {

	class Random
	{
	public:
		static void Init();

		/*
		 * @return A random 32 bit integer.
		 */
		static int Int();

		/*
		 * @return A random unsignend 16 bit integer.
		 */
		static uint16_t UInt16();

		/*
		 * Returns a random unsignend 16 bit integer between min and max. Inclusive.
		 */
		static uint16_t UInt16(uint16_t min, uint16_t max);

		/*
		 * @return A random unsignend 32 bit integer.
		 */
		static uint32_t UInt32();

		/*
		 * Returns a random unsignend 32 bit integer between min and max. Inclusive.
		 */
		static uint32_t UInt32(uint32_t min, uint32_t max);

		/*
		 * @return A random unsignend 64 bit integer.
		 */
		static uint64_t UInt64();

		/*
		 * Returns a random unsignend 64 bit integer between min and max. Inclusive.
		 */
		static uint64_t UInt64(uint64_t min, uint64_t max);

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
		static std::uniform_int_distribution<int> s_IntDistribution;
		static std::uniform_int_distribution<uint16_t> s_UIntDistribution16;
		static std::uniform_int_distribution<uint32_t> s_UIntDistribution32;
		static std::uniform_int_distribution<uint64_t> s_UIntDistribution64;
		static std::uniform_int_distribution<std::mt19937::result_type> s_DistributionFloat;
		static std::bernoulli_distribution s_BoolDistribution;

	};

}
