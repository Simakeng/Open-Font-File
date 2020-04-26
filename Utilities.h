#pragma once
#include "Exceptions.h"
namespace OpenFont
{
	template <typename T>
	auto BELEConv(const T& rhs)
	{
		T res;
		auto src = reinterpret_cast<const uint8_t*>(&rhs);
		auto dst = reinterpret_cast<uint8_t*>(&res);
		for (size_t i = 0; i < sizeof(T); i++)
			dst[i] = src[sizeof(T) - i - 1];
		return res;
	}

	template<typename T, class stream >
	T Read(stream s)
	{
		T res;
		auto bc = fread(&res, 1, sizeof(T), s);
		if (bc != sizeof(T))
			throw FileIOException("End of file too early!");
		return res;
	}

	int MaxPowerOf2(int rhs)
	{
		int i = 0;
		int v = rhs;
		while (v >>= 1)
			i = i << 1 | 1;
		return i + 1;
	};

}