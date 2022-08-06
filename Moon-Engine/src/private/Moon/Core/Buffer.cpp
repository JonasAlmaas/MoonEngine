#include "mepch.h"
#include "Moon/Core/Buffer.h"


namespace Moon {

	Buffer::Buffer()
	: Data(nullptr), Size(0)
	{
	}

	Buffer::Buffer(void* data, uint32_t size)
		: Data(data), Size(size)
	{
	}

	Buffer Buffer::Copy(const void* data, uint32_t size)
	{
		Buffer buffer;
		buffer.Allocate(size);
		memcpy(buffer.Data, data, size);
		return buffer;
	}

	void Buffer::Allocate(uint32_t size)
	{
		delete[](byte*)Data;
		Data = nullptr;

		if (size == 0)
			return;

		Data = new byte[size];
		Size = size;
	}

	void Buffer::Release()
	{
		delete[](byte*)Data;
		Data = nullptr;
		Size = 0;
	}

	void Buffer::ZeroInitialize()
	{
		if (Data)
			memset(Data, 0, Size);
	}

	byte* Buffer::ReadBytes(uint32_t size, uint32_t offset)
	{
		ME_CORE_ASSERT(offset + size <= Size, "Buffer overflow!");
		byte* buffer = new byte[size];
		memcpy(buffer, (byte*)Data + offset, size);
		return buffer;
	}

	void Buffer::Write(const void* data, uint32_t size, uint32_t offset)
	{
		ME_CORE_ASSERT(offset + size <= Size, "Buffer overflow!");
		memcpy((byte*)Data + offset, data, size);
	}

	BufferSafe::~BufferSafe()
	{
		Release();
	}

	BufferSafe BufferSafe::Copy(const void* data, uint32_t size)
	{
		BufferSafe buffer;
		buffer.Allocate(size);
		memcpy(buffer.Data, data, size);
		return buffer;
	}

}
