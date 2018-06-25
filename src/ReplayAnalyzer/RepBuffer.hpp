#pragma once

#include <stdint.h>
#include <string>

namespace WRep
{
	class RepBuffer
	{
	private:
		std::string m_buf;
		uint32_t    m_pos;

	public:
		RepBuffer();
		RepBuffer(std::string&& buf);
		std::string ReadBytes(uint32_t n);
		void        Seek(int pos);
		uint8_t     Peek() const;
		void        Write(char* buf, uint32_t size);
		bool        IsEnd();
		std::string ReadString();
		std::string GetDecodedBytes();

		template<class _struct>
		_struct* Read()
		{
			_struct* pRet = (_struct*)&(m_buf[m_pos]);
			m_pos += sizeof(_struct);
			return pRet;
		}

		template<class _struct>
		_struct* Peek() const
		{
			return (_struct*)&(m_buf[m_pos]);
		}
	};
}
