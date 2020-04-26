#pragma once
#include <exception>
namespace OpenFont
{
	namespace Exceptions
	{
		class FileAccessException : public std::exception
		{
		public:
			FileAccessException(const char* filename);
			~FileAccessException() = default;
		};
	}
	using FileAccessException = Exceptions::FileAccessException;
	using FileIOException = std::exception;
	using FontTypeException = std::exception;
}