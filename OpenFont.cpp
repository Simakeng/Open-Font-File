#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <io.h>
#include <utility>
#include <deque>

#include "Types.h"
#include "OpenFont.h"
#include "Utilities.h"
#include "Exceptions.h"

#ifdef _WIN32
#define access _access
#endif

void CheckOffsetTable(OpenFont::OffsetTable& table)
{
	using namespace OpenFont;
	if (table.sfntVersion != 0x00010000 and table.sfntVersion != 0x4F54544F)
		goto CheckOffsetTable_exception;

	if (MaxPowerOf2(table.numTables) != table.searchRange / 16)
		goto CheckOffsetTable_exception;

	if (log2(MaxPowerOf2(table.numTables)) != table.entrySelector)
		goto CheckOffsetTable_exception;

	if ((table.numTables * 16 - table.searchRange) != table.rangeShift)
		goto CheckOffsetTable_exception;

	return;

CheckOffsetTable_exception:
	throw OpenFont::FontTypeException("Not a valid Open Type Font file!");
}

namespace OpenFont
{
	Font::Font(const char* fontFileName)
	{
		FILE* fp = nullptr;
		try
		{
			if (access(fontFileName, 4) != 0)
				throw FileAccessException(fontFileName);
			fp = fopen(fontFileName, "rb");

			auto offsetTable = Read<OffsetTable>(fp);
			CheckOffsetTable(offsetTable);

			std::deque<TableRecord> tablesRecs;
			for (int i = 0; i < offsetTable.numTables; i++)
				tablesRecs.emplace_back(Read<TableRecord>(fp));

			for (auto& tableRec : tablesRecs)
			{
				u32 offset = tableRec.offset;
				u32 length = tableRec.length;
				Table table = { tableRec , length };
				fseek(fp, offset, SEEK_SET);
				if (fread(table.tableData, 1, length, fp) != length)
					throw FileIOException("End of file too early!");
				tables.emplace_back(std::move(table));
			}

			if (fp != nullptr)
				fclose(fp);

			for (auto& table : this->tables)
			{
				if (table.tableRecord.tableTag != "cmap")
					continue;
				using namespace cmap;
				auto& data = table.tableData;
				TableHeader& header = *(TableHeader*)(data);
				EncodingRecorder* record = (EncodingRecorder*)(data + sizeof(TableHeader));

				for (auto i = 0; i < header.numTable; i++)
				{
					if (record[i].PlatformID != PlatformID::Unicode)
						continue;
					if (record[i].encodingID != EncodingID::Unicode::Unicode_BMP)
						continue;

					auto off = record[i].offset;
					printf("platformID: %d\n", (int)record[i].PlatformID);
					printf("encodingID: %d\n", (int)record[i].encodingID);
					printf("===========\n");
					bu16& subTableFormat = *(bu16*)(data + off);
					printf("tableFormat: %d\n", (int)subTableFormat);

				}
			}
		}
		catch (const std::exception&)
		{
			if (fp != nullptr)
				fclose(fp);
			throw;
		}
	}
}