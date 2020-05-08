#include <exception>
#include <iostream>
#include "OpenFont.h"
using namespace OpenFont;
int main()
{
	try
	{
		auto font = Font(R"(C:\Windows\Fonts\SourceHanSansSC-Regular.otf)");
	
	}
	catch (const std::exception& exc)
	{
		std::cout << "Exception Occured!\n";
		std::cout << exc.what() << std::endl;
	}
}