#pragma once

#include <exception>
#include <cstdarg>
#include <cstdio>
#include <malloc.h>

namespace WRep
{
	class Exception : public std::exception
	{
	public:
		explicit Exception()
			: what_(nullptr)
		{ }

		explicit Exception(const char* format, ...)
			: what_(nullptr)
		{
			va_list ap;
			va_start(ap, format);
			what_ = getFormatString(format, ap);
			va_end(ap);
		}

		~Exception() throw()
		{
			::free(const_cast<char*>(what_));
			what_ = nullptr;
		}

		const char* what() const throw()
		{
			return what_ ? what_ : "Unknown SLK::Exception";
		}

	private:
		const char* getFormatString(char const* fmt, va_list argsList)
		{
			size_t buffer_size = ::_vscprintf(fmt, argsList) + 1;

			char* buffer = static_cast<char *>(::malloc(buffer_size));

			if (buffer)
			{
#ifdef _MSC_VER
				int n = ::_vsnprintf_s(buffer, buffer_size, buffer_size, fmt, argsList);
#else
				int n = ::_vsnprintf(buffer, buffer_size, fmt, argsList);
#endif
				if (n > 0)
				{
					return buffer;
				}
			}

			return nullptr;
		}

		const char * what_;
	};
}
