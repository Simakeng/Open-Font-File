#include "Types.h"
namespace OpenFont
{
	TableData::TableData(size_t length)
		:pData(new uint8_t[length])
	{

	}
	TableData::TableData(TableData&& rhs)
		: pData(rhs.pData)
	{
		rhs.pData = nullptr;
	}
	TableData::~TableData()
	{
		if (pData != nullptr)
			delete[] pData;
	}
}

