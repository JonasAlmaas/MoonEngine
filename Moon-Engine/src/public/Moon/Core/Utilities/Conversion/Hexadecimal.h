#pragma once

#include "Moon/Core/Base.h"


namespace Moon {

	class Hex
	{
	public:
		static std::string ByteToHex(uint8_t byte);
		static uint8_t HexToByte(const std::string& hex);

		static uint8_t DesimalLookup(uint8_t hex);
		static uint8_t HexLookup(uint8_t desimal);

	};

}
