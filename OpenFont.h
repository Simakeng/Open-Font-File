#pragma once
#include <deque>
#include "Types.h"

namespace OpenFont 
{
	class Font 
	{
	public:
		Font(const char* fontFileName);
		~Font() = default;
	private:
		std::deque<Table> tables;
	};
}