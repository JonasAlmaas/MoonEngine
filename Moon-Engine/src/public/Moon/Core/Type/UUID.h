#pragma once


namespace Moon {

	class UUID
	{
	public:
		UUID();
		UUID(uint64_t uuid);
		UUID(const std::string& uuid);

		UUID(const UUID&) = default;

		std::string ToHexString();

	public:
		virtual bool operator==(const UUID& other) const { return m_UUID == other.m_UUID; }
		virtual bool operator!=(const UUID& other) const { return !(*this == other); }

		operator uint64_t() const { return m_UUID; }

	private:
		uint64_t m_UUID;

	};

}

namespace std {

	template <typename T> struct hash;

	template<>
	struct hash<Moon::UUID>
	{
		std::size_t operator()(const Moon::UUID& uuid) const
		{
			return (uint64_t)uuid;
		}
	};

}
