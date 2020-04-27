#pragma once
#include <cstdint>
#include "Utilities.h"
namespace OpenFont
{
	using char_type = wchar_t;

	template<typename T>
	struct BigEndingType
	{
		T value;
		operator T() {
			return BELEConv(value);
		}
		template<typename T>
		T* data()
		{
			return reinterpret_cast<T*>(value);
		}
	};

	using i32 = int32_t;
	using bi32 = BigEndingType<int32_t>;

	using i16 = int16_t;
	using bi16 = BigEndingType<int16_t>;

	using u32 = uint32_t;
	using bu32 = BigEndingType<uint32_t>;

	using u16 = uint16_t;
	using bu16 = BigEndingType<uint16_t>;

	using u8 = uint8_t;

	using tag = u8[4];

	struct OffsetTable
	{
		bu32 sfntVersion;
		bu16 numTables;
		bu16 searchRange;
		bu16 entrySelector;
		bu16 rangeShift;
	};

	struct TableRecord
	{
		tag tableTag;
		bu32 checksum;
		bu32 offset;
		bu32 length;
	};

	class TableData
	{
	private:
		uint8_t* pData;
	public:
		TableData(size_t length);
		TableData(TableData&& rhs);
		~TableData();
		template<typename T>
		operator T* () { return reinterpret_cast<T*>(pData); }
	};

	struct Table
	{
		TableRecord tableRecord;
		TableData tableData;
	};
}
