#include "RepBuffer.hpp"
#include <deque>

namespace WRep
{
	RepBuffer::RepBuffer()
		: m_pos(0)
	{ }

	RepBuffer::RepBuffer(std::string&& buf)
		: m_buf(std::move(buf))
		, m_pos(0)
	{ }

	std::string RepBuffer::ReadBytes(uint32_t n)
	{
		m_pos += n;
		return m_buf.substr(m_pos-n, n);
	}

	void RepBuffer::Seek(int pos)
	{
		this->m_pos = pos;
	}

	uint8_t RepBuffer::Peek() const
	{
		return (uint8_t)(m_buf[m_pos]);
	}

	void RepBuffer::Write(char* buf, uint32_t size)
	{
		m_buf.append(buf, size);
	}

	bool RepBuffer::IsEnd()
	{
		return (m_buf.size() <= m_pos);
	}

	std::string RepBuffer::ReadString()
	{
		uint32_t start = m_pos;

		while (m_buf[m_pos++] != '\0')
			NULL;

		return std::move(std::string(m_buf, start, m_pos - start));
	}

	std::string RepBuffer::GetDecodedBytes()
	{
		std::deque<uint8_t> decoded;
		int pos = 0;
		uint8_t mask = 0;

		uint8_t b = *(Read<uint8_t>());
		while (b != 0)
		{
			if (pos % 8 == 0)
			{
				mask = b;
			}
			else
			{
				if ((mask & (0x1 << (pos % 8))) == 0)
					decoded.push_back((uint8_t)(b - 1));
				else
					decoded.push_back(b);
			}

			b = *(Read<uint8_t>());
			pos++;
		}
		return std::move(std::string(decoded.begin(), decoded.end()));
	}
}
