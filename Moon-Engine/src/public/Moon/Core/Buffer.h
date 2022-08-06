#pragma once

#include "Moon/Core/Base.h"


namespace Moon {

	class Buffer
	{
	public:
		void* Data;
		uint32_t Size;

	public:
		Buffer();
		Buffer(void* data, uint32_t size);

		void Allocate(uint32_t size);
		void Release();
		void ZeroInitialize();

		template<typename T>
		T& Read(uint32_t offset = 0)
		{
			return *(T*)((byte*)Data + offset);
		}

		byte* ReadBytes(uint32_t size, uint32_t offset);
		void Write(const void* data, uint32_t size, uint32_t offset = 0);

		inline uint32_t GetSize() const { return Size; }

	public:
		operator bool() const
		{
			return Data;
		}

		byte& operator[](int index)
		{
			return ((byte*)Data)[index];
		}

		template<typename T>
		T* As() const
		{
			return (T*)Data;
		}

	public:
		static Buffer Copy(const void* data, uint32_t size);

	};

	class BufferSafe : public Buffer
	{
	public:
		~BufferSafe();
		static BufferSafe Copy(const void* data, uint32_t size);

	};

}
