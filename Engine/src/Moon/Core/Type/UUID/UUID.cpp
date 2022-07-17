#include "mepch.h"
#include "Moon/Core/Type/UUID/UUID.h"

#include "Moon/Core/Type/Hex/Hex.h"

#include <random>


namespace Moon {

	static std::random_device s_RandomDevice;
	static std::mt19937_64 s_Engine(s_RandomDevice());
	static std::uniform_int_distribution<uint64_t> s_UniformDistribution;

	UUID::UUID()
		: m_UUID(s_UniformDistribution(s_Engine))
	{
	}

	UUID::UUID(uint64_t uuid)
		: m_UUID(uuid)
	{
	}

	std::string UUID::ToString()
	{
		std::string result;
		for (int8_t byteIndex = 7; byteIndex >= 0; byteIndex--)
			result += Hex::ByteToHex(BYTE_VALUE(m_UUID, byteIndex));

		return result;
	}

}
