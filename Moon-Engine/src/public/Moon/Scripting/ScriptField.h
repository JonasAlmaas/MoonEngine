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
			static_assert(sizeof(T) <= 8, "Typer too large!");
			return *(T*)m_Buffer;
		}

		template<typename T>
		void SetValue(T value)
		{
			static_assert(sizeof(T) <= 8, "Typer too large!");
			memcpy(m_Buffer, &value, sizeof(T));
		}

	private:
		uint8_t m_Buffer[8];

	private:
		friend class ScriptEngine;
		friend class ScriptInstance;

	};

	using ScriptFieldMap = std::unordered_map<std::string, ScriptFieldInstance>;

}
