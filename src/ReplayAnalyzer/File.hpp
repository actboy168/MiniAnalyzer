#pragma once

#include <fstream>
#include <string>
#include "Exception.hpp"

namespace WRep
{
	namespace detail
	{
		class File
		{
		private:
			std::fstream file_;

		public:
			File(const char* _Filename, std::ios_base::openmode _Mode)
			{
				file_.open(_Filename, std::ios::binary | _Mode);
			}

			File(const wchar_t* _Filename, std::ios_base::openmode _Mode)
			{
				file_.open(_Filename, std::ios::binary | _Mode);
			}

			~File() 
			{ 
				file_.close();
			}

			bool is_open() const
			{
				return !!file_;
			}

			template <class SequenceT>
			SequenceT Reads()
			{
				return std::move(SequenceT((std::istreambuf_iterator<char>(file_)), (std::istreambuf_iterator<char>())));
			}

			template <class SequenceT>
			void Writes(SequenceT buf)
			{
				std::copy(buf.begin(), buf.end(), std::ostreambuf_iterator<char>(file_));
			}
			
		private:
			File();
		};
	}

	class WriteFile
	{
	public:
		WriteFile(const char* _Filename)
			: file_(_Filename, std::ios_base::out)
		{
			if (!file_.is_open())
			{
				throw Exception("Can not open the file %s.", _Filename);
			}
		}

		WriteFile(const std::string& _Str)
			: file_(_Str.c_str(), std::ios_base::out)
		{
			if (!file_.is_open())
			{
				throw Exception("Can not open the file %s.", _Str.c_str());
			}
		}

		WriteFile(const wchar_t* _Filename)
			: file_(_Filename, std::ios_base::out)
		{ 
			if (!file_.is_open())
			{
				throw Exception("Can not open the file %S.", _Filename);
			}
		}

		WriteFile(const std::wstring& _Str)
			: file_(_Str.c_str(), std::ios_base::out)
		{ 
			if (!file_.is_open())
			{
				throw Exception("Can not open the file %S.", _Str.c_str());
			}
		}

		template <class SequenceT>
		void Writes(SequenceT buf)
		{
			file_.Writes<SequenceT>(buf);
		}

	private:
		detail::File file_;
	};

	class ReadFile
	{
	public:
		ReadFile(const char* _Filename)
			: file_(_Filename, std::ios_base::in)
		{
			if (!file_.is_open())
			{
				throw Exception("Can not open the file %s.", _Filename);
			}
		}

		ReadFile(const std::string& _Str)
			: file_(_Str.c_str(), std::ios_base::in)
		{
			if (!file_.is_open())
			{
				throw Exception("Can not open the file %s.", _Str.c_str());
			}
		}

		ReadFile(const wchar_t* _Filename)
			: file_(_Filename, std::ios_base::in)
		{
			if (!file_.is_open())
			{
				throw Exception("Can not open the file %S.", _Filename);
			}
		}

		ReadFile(const std::wstring& _Str)
			: file_(_Str.c_str(), std::ios_base::in)
		{
			if (!file_.is_open())
			{
				throw Exception("Can not open the file %S.", _Str.c_str());
			}
		}

		template <class SequenceT>
		SequenceT Reads()
		{
			return std::move(file_.Reads<SequenceT>());
		}

	private:
		detail::File file_;
	};
}
