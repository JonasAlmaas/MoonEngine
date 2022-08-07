#include "mepch.h"
#include "Moon/Core/Type/UUID.h"

#include "Moon/Core/Utilities/Conversion/Hexadecimal.h"


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

	UUID::UUID(const std::string& uuid)
		: m_UUID(0)
	{
		for (uint8_t byteIndex = 0; byteIndex < 8; byteIndex++)
		{
			const char hex[3] = { uuid[byteIndex * 2], uuid[byteIndex * 2 + 1], 0 };
			uint8_t byteValue = Hex::HexToByte(hex);
			m_UUID |= ((uint64_t)byteValue) << ((7 - byteIndex) * 8);
		}
	}

	std::string UUID::ToHexString()
	{
		std::string result;
		for (int8_t byteIndex = 7; byteIndex >= 0; byteIndex--)
			result += Hex::ByteToHex(BYTE_VALUE(m_UUID, byteIndex));

		return result;
	}

}
