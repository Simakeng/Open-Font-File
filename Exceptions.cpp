#include "Exceptions.h"
#include <string>
namespace OpenFont
{
	namespace Exceptions
	{
		auto MakeFileAccessException(const char* filename)
		{
			return std::string("File '") + filename + "' Not found!";
		}

		FileAccessException::FileAccessException(const char* filename)
			: std::exception(MakeFileAccessException(filename).data())
		{

		}
	}
}