#pragma once
#include <cstdint>
#include <cstring>
#include "Utilities.h"

namespace OpenFont
{
	using char_type = wchar_t;

	template<typename T>
	struct BigEndingType
	{
		using prototype = T;
		T value;
		operator T() {
			return BELEConv(value);
		}
		operator const T() const{
			return BELEConv(value);
		}
		template<typename T>
		T* data()
		{
			return reinterpret_cast<T*>(value);
		}
	};

	template<typename T, typename enumType>
	inline bool operator==(const enumType& lhs, const BigEndingType<T>& rhs)
	{
		return (static_cast<const T>(lhs) == static_cast<const T>(rhs));
	}
	template<typename T, typename enumType>
	inline bool operator!=(const enumType& lhs, const BigEndingType<T>& rhs)
	{
		return (static_cast<const T>(lhs) != static_cast<const T>(rhs));
	}

	template<typename T,typename enumType>
	inline bool operator==(const BigEndingType<T>& lhs, const enumType& rhs)
	{
		return (static_cast<const T>(lhs) == static_cast<const T>(rhs));
	}
	template<typename T, typename enumType>
	inline bool operator!=(const BigEndingType<T>& lhs, const enumType& rhs)
	{
		return (static_cast<const T>(lhs) != static_cast<const T>(rhs));
	}


	using i32 = int32_t;
	using bi32 = BigEndingType<int32_t>;

	using i16 = int16_t;
	using bi16 = BigEndingType<int16_t>;

	using u32 = uint32_t;
	using bu32 = BigEndingType<uint32_t>;

	using u16 = uint16_t;
	using bu16 = BigEndingType<uint16_t>;

	using u8 = uint8_t;

	//   |     |  |
	// 0x 12 34 56  big endian
	// 0x 56 34 12  little endian

#pragma pack(push,1)
	struct u24
	{
		u16 h;
		u8 l;
	};
	struct bu24
	{
		u8 l;
		bu16 h;
	};
#pragma pack(pop)

	struct tag
	{
		u8 data[4];
		operator u8* () { return data; }
		operator u32 () { return *reinterpret_cast<u32*>(data); }
		bool operator ==(const char* str) { return memcmp(data, str, 4) == 0; }
		bool operator !=(const char* str) { return memcmp(data, str, 4) != 0; }
	};

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
		u8* operator+ (int i) { return reinterpret_cast<u8*>(pData) + i; }
	};

	struct Table
	{
		TableRecord tableRecord;
		TableData tableData;
	};
	namespace cmap
	{
		struct TableHeader
		{
			bu16 Version;
			bu16 numTable;
		};

		struct EncodingRecorder
		{
			bu16 PlatformID;
			bu16 encodingID;
			bu32 offset;
		};

		enum class PlatformID : bu16::prototype
		{
			Unicode = 0,
			Macintosh = 1,
			ISO = 2,
			Windows = 3,
			Custom = 4
		};

		namespace EncodingID 
		{
			enum class Unicode : bu16::prototype
			{
				Unicode_1_0 = 0,	//<! Unicode 1.0 semantics 
				Unicode_1_1 = 1,	//<! Unicode 1.1 semantics 
				ISO_IEC_10646 = 2,	//<! ISO/IEC 10646 semantics 
				Unicode_BMP = 3,	//<! Unicode 2.0 and onwards semantics, Unicode BMP only (cmap subtable formats 0, 4, 6).
				Unicode_2_0 = 4,	//<! Unicode 2.0 and onwards semantics, Unicode full repertoire (cmap subtable formats 0, 4, 6, 10, 12).
				UnicodeVS = 5,		//<! Unicode Variation Sequences (cmap subtable format 14). 
				UnicodeFR = 6		//<! Unicode full repertoire (cmap subtable formats 0, 4, 6, 10, 12, 13). 
			};

			enum class Windows : bu16::prototype
			{
				Symbol = 0,
				Unicode_BMP = 1,
				UCS_2 = 1,
				Shift_JIS = 2,
				PRC = 3,
				Big5 = 4,
				Wansung = 5,
				Johab = 6,
				UCS_4 = 10
			};
		}

		

		

		namespace Subtables
		{
#pragma pack(push,1)
			// Each table
			class BasicSubtable 
			{
			public:
				BasicSubtable(void* bufferData);
				~BasicSubtable();
			protected:
				bu16 format;
				bu16 length;
			};
			/**
				Macintosh platform standard character to glyph index mapping table
			*/
			class F0 : BasicSubtable
			{
				bu16 language;
				u8 glyphIdArray[256];
			};
			/**
				High byte mapping through table
				This subtable is useful for the national character code standards used for Japanese, Chinese, and Korean
				characters. These code standards use a mixed 8/16-bit encoding, in which certain byte values signal the first
				byte of a 2-byte character (but these values are also legal as the second byte of a 2-byte character).
			*/ 
			class F2 : BasicSubtable
			{
				bu16 language;
				bu16 subHeaderKeys[256];

				struct SubHeader 
				{
					bu16 firstCode;
					bu16 entryCount;
					bi16 idDelta;
					bu16 idRangeOffset;
				};

				SubHeader* subHeaders;
				bu16* glyphIndexArray;
			};

			class F4 : BasicSubtable
			{
				bu16 language;
				bu16 segCountX2;
				bu16 searchRange;
				bu16 entrySelector;
				bu16 rangeShift;
			};


#pragma pack (pop)
		}
	}
}
