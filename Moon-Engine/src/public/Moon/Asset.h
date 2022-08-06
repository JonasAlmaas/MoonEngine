#pragma once

#include "Moon/Asset/AssetTypes.h"
#include "Moon/Core/Type/UUID.h"


namespace Moon {

	using AssetHandle = UUID;

	class Asset
	{
	public:
		Asset() { m_AssetHandle = AssetHandle(); }
		virtual ~Asset() = default;

		bool IsValid() const { return ((m_Flags & (uint16_t)AssetFlag::Missing) | (m_Flags & (uint16_t)AssetFlag::Invalid)) == 0; }

		virtual AssetType GetAssetType() const { return AssetType::None; }

		bool IsFlagSet(AssetFlag flag) const { return (uint16_t)flag & m_Flags; }
		void SetFlag(AssetFlag flag, bool value = true)
		{
			if (value)
				m_Flags |= (uint16_t)flag;
			else
				m_Flags &= ~(uint16_t)flag;
		}

	public:
		virtual bool operator==(const Asset& other) const { return m_AssetHandle == other.m_AssetHandle; }
		virtual bool operator!=(const Asset& other) const { return !(*this == other); }

	protected:
		AssetHandle m_AssetHandle = 0;
		uint16_t m_Flags = (uint16_t)AssetFlag::None;

	};

}
