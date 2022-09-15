#pragma once


extern "C" {
	typedef struct _MonoClassField MonoClassField;
}

namespace Moon {

	enum class ScriptFieldType
	{
		None = 0,
		Float, Float2, Float3, Float4,
		Double,
		Bool, Char, Byte, Short, Int, Long,
		UByte, UShort, UInt, ULong,
		Entity
	};

	struct ScriptField
	{
		ScriptFieldType Type;
		std::string Name;

		MonoClassField* ClassField;
	};

	struct ScriptFieldInstance
	{
		ScriptField Field;

		ScriptFieldInstance()
		{
			memset(m_Buffer, 0, sizeof(m_Buffer));
		}

		template<typename T>
		T GetValue()
		{
			static_assert(sizeof(T) <= 16, "Typer too large!");
			return *(T*)m_Buffer;
		}

		template<typename T>
		void SetValue(T value)
		{
			static_assert(sizeof(T) <= 16, "Typer too large!");
			memcpy(m_Buffer, &value, sizeof(T));
		}

	private:
		uint8_t m_Buffer[16];

	private:
		friend class ScriptEngine;
		friend class ScriptInstance;

	};

	using ScriptFieldMap = std::unordered_map<std::string, ScriptFieldInstance>;

	namespace Utils {

		inline const char* ScriptFieldTypeToString(ScriptFieldType type)
		{
			switch (type)
			{
			case ScriptFieldType::None:		return "None";
			case ScriptFieldType::Float:	return "Float";
			case ScriptFieldType::Float2:	return "Float2";
			case ScriptFieldType::Float3:	return "Float3";
			case ScriptFieldType::Float4:	return "Float4";
			case ScriptFieldType::Double:	return "Double";
			case ScriptFieldType::Bool:		return "Bool";
			case ScriptFieldType::Char:		return "Char";
			case ScriptFieldType::Byte:		return "Byte";
			case ScriptFieldType::Short:	return "Short";
			case ScriptFieldType::Int:		return "Int";
			case ScriptFieldType::Long:		return "Long";
			case ScriptFieldType::UByte:	return "UByte";
			case ScriptFieldType::UShort:	return "UShort";
			case ScriptFieldType::UInt:		return "UInt";
			case ScriptFieldType::ULong:	return "ULong";
			case ScriptFieldType::Entity:	return "Entity";
			}

			return "<Invalid>";
		}

		inline ScriptFieldType ScriptFieldTypeFromString(std::string_view fieldType)
		{
			if (fieldType == "None")		return ScriptFieldType::None;
			if (fieldType == "Float")		return ScriptFieldType::Float;
			if (fieldType == "Float2")		return ScriptFieldType::Float2;
			if (fieldType == "Float3")		return ScriptFieldType::Float3;
			if (fieldType == "Float4")		return ScriptFieldType::Float4;
			if (fieldType == "Double")		return ScriptFieldType::Double;
			if (fieldType == "Bool")		return ScriptFieldType::Bool;
			if (fieldType == "Char")		return ScriptFieldType::Char;
			if (fieldType == "Byte")		return ScriptFieldType::Byte;
			if (fieldType == "Short")		return ScriptFieldType::Short;
			if (fieldType == "Int")			return ScriptFieldType::Int;
			if (fieldType == "Long")		return ScriptFieldType::Long;
			if (fieldType == "UByte")		return ScriptFieldType::UByte;
			if (fieldType == "UShort")		return ScriptFieldType::UShort;
			if (fieldType == "UInt")		return ScriptFieldType::UInt;
			if (fieldType == "ULong")		return ScriptFieldType::ULong;
			if (fieldType == "Entity")		return ScriptFieldType::Entity;

			ME_CORE_ASSERT(false, "Unknown ScriptFieldType!");
			return ScriptFieldType::None;
		}

	}

}
