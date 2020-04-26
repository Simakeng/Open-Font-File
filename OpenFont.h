#pragma once
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