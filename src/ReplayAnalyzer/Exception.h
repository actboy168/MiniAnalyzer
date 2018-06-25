#pragma once

#include <exception>

namespace WRep
{
	class Exception : public std::exception 
	{   
	public:
		explicit Exception() 
			: id_(0) 
		{ }

		explicit Exception(int err_id)
			: id_(err_id)
		{ }

		virtual ~Exception() throw () 
		{ }

		virtual const char* what() const throw () 
		{
			return "WRep::Exception";
		}

		int id() const throw () 
		{
			return id_;
		}

	private:
		int id_;
	};
}
