#include "mepch.h"
#include "Moon/Core/Utilities/Conversion/Hexadecimal.h"


namespace Moon {

	std::string Hex::ByteToHex(uint8_t byte)
	{
		std::string result;
		result += HexLookup(BIT_VALUE_RANGE(byte, 4, 7));
		result += HexLookup(BIT_VALUE_RANGE(byte, 0, 3));
		return result;
	}

	uint8_t Hex::HexToByte(const std::string& hex)
	{
		return DesimalLookup(hex[1]) | (DesimalLookup(hex[0]) << 4);
	}

	uint8_t Hex::DesimalLookup(uint8_t hex)
	{
		switch (hex)
		{
			case 48:				return 0;
			case 49:				return 1;
			case 50:				return 2;
			case 51:				return 3;
			case 52:				return 4;
			case 53:				return 5;
			case 54:				return 6;
			case 55:				return 7;
			case 56:				return 8;
			case 57:				return 9;
			case 65: case 97:		return 10;
			case 66: case 98:		return 11;
			case 67: case 99:		return 12;
			case 68: case 100:		return 13;
			case 69: case 101:		return 14;
			case 70: case 102:		return 15;
		}

		ME_CORE_ASSERT(false, "Value is not recoginces as a hexadecimal value!");
		return 0;
	}

	uint8_t Hex::HexLookup(uint8_t desimal)
	{
		switch (desimal)
		{
			case 0:		return 48;
			case 1:		return 49;
			case 2:		return 50;
			case 3:		return 51;
			case 4:		return 52;
			case 5:		return 53;
			case 6:		return 54;
			case 7:		return 55;
			case 8:		return 56;
			case 9:		return 57;
			case 10:	return 97;
			case 11:	return 98;
			case 12:	return 99;
			case 13:	return 100;
			case 14:	return 101;
			case 15:	return 102;
		}

		ME_CORE_ASSERT(false, "Value is bigger than 15 and can not convert to hexadecimal!");
		return 0;
	}

}
