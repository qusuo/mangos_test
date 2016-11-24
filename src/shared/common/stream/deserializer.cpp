#include "deserializer.h"
#include <memory>
namespace jxsstr
{
Deserializer::Deserializer()
	:m_data_buf(NULL)
	,m_data_len(0)
	,m_read_pos(0)
{
}

Deserializer::Deserializer( char *buf, Int32 data_len )
	:m_data_buf(buf)
	,m_data_len(data_len)
	,m_read_pos(0)
{

}

void Deserializer::Reset( char *buf, Int32 data_len )
{
	m_data_buf = buf;
	m_data_len = data_len;
	m_read_pos = 0;
}

Int32 Deserializer::GetDataLen()
{
	return m_data_len;
}

char* Deserializer::GetData()
{
	return m_data_buf;
}

bool Deserializer::ViewInt8(Int8& v)
{
	Int32 read_len = sizeof(v);
	if (m_read_pos + read_len > m_data_len)
	{
		return false;
	}
	else
	{
		memcpy((char*)&v, m_data_buf + m_read_pos, read_len);
		return true;
	}
}

bool Deserializer::ViewInt16(Int16& v)
{
	Int32 read_len = sizeof(v);
	if (m_read_pos + read_len > m_data_len)
	{
		return false;
	}
	else
	{
		memcpy((char*)&v, m_data_buf + m_read_pos, read_len);
		return true;
	}
}

bool Deserializer::ViewInt32(Int32& v)
{
	Int32 read_len = sizeof(v);
	if (m_read_pos + read_len > m_data_len)
	{
		return false;
	}
	else
	{
		memcpy((char*)&v, m_data_buf + m_read_pos, read_len);
		return true;
	}
}

bool Deserializer::ViewInt32(Int32 index, Int32& v)
{
	Int32 read_len = sizeof(v);
	if (m_read_pos + read_len + index * read_len> m_data_len)
	{
		return false;
	}
	else
	{
		memcpy((char*)&v, m_data_buf + m_read_pos + index * read_len, read_len);
		return true;
	}
}

bool Deserializer::GetInt8( Int8& v )
{
	Int32 read_len = sizeof(v);
	if (m_read_pos + read_len > m_data_len)
	{
		return false;
	}
	else
	{
		memcpy((char*)&v, m_data_buf + m_read_pos, read_len);
		m_read_pos += read_len;
		return true;
	}
}

bool Deserializer::GetInt16( Int16& v )
{
	Int32 read_len = sizeof(v);
	if (m_read_pos + read_len > m_data_len)
	{
		return false;
	}
	else
	{
		memcpy((char*)&v, m_data_buf + m_read_pos, read_len);
		m_read_pos += read_len;
		return true;
	}
}

bool Deserializer::GetInt32( Int32& v )
{
	Int32 read_len = sizeof(v);
	if (m_read_pos + read_len > m_data_len)
	{
		return false;
	}
	else
	{
		memcpy((char*)&v, m_data_buf + m_read_pos, read_len);
		m_read_pos += read_len;
		return true;
	}
}

bool Deserializer::GetInt64( Int64 &v )
{
	Int32 read_len = sizeof(v);
	if (m_read_pos + read_len > m_data_len)
	{
		return false;
	}
	else
	{
		memcpy((char*)&v, m_data_buf + m_read_pos, read_len);
		m_read_pos += read_len;
		return true;
	}
}

bool Deserializer::GetInt8s( Int8 * v, Int32 vl )
{
	Int32 read_len = sizeof(*v) * vl;
	if (m_read_pos + read_len > m_data_len)
	{
		return false;
	}
	else
	{
		memcpy((char*)v, m_data_buf + m_read_pos, read_len);
		m_read_pos += read_len;
		return true;
	}
}

bool Deserializer::GetInt16s( Int16 * v, Int32 vl )
{
	Int32 read_len = sizeof(*v) * vl;
	if (m_read_pos + read_len > m_data_len)
	{
		return false;
	}
	else
	{
		memcpy((char*)v, m_data_buf + m_read_pos, read_len);
		m_read_pos += read_len;
		return true;
	}
}

bool Deserializer::GetInt32s( Int32 * v, Int32 vl )
{
	Int32 read_len = sizeof(*v) * vl;
	if (m_read_pos + read_len > m_data_len)
	{
		return false;
	}
	else
	{
		memcpy((char*)v, m_data_buf + m_read_pos, read_len);
		m_read_pos += read_len;
		return true;
	}
}

bool Deserializer::GetInt64s( Int64 * v, Int32 vl )
{
	Int32 read_len = sizeof(*v) * vl;
	if (m_read_pos + read_len > m_data_len)
	{
		return false;
	}
	else
	{
		memcpy((char*)v, m_data_buf + m_read_pos, read_len);
		m_read_pos += read_len;
		return true;
	}
}

bool Deserializer::GetInt8Vector(Int8 * v, Int16 max_len, Int16& out_len)
{
	if (!GetInt16(out_len)
		|| out_len > max_len)
	{
		return false;
	}

	return GetInt8s(v, out_len);
}

bool Deserializer::GetInt16Vector(Int16 * v, Int32 max_len, Int16& out_len)
{
	if (!GetInt16(out_len)
		|| out_len > max_len)
	{
		return false;
	}

	return GetInt16s(v, out_len);
}

bool Deserializer::GetInt32Vector(Int32 * v, Int32 max_len, Int16& out_len)
{
	if (!GetInt16(out_len)
		|| out_len > max_len)
	{
		return false;
	}

	return GetInt32s(v, out_len);
}

bool Deserializer::GetInt64Vector(Int64 * v, Int32 max_len, Int16& out_len)
{
	if (!GetInt16(out_len)
		|| out_len > max_len)
	{
		return false;
	}

	return GetInt64s(v, out_len);
}


void Deserializer::MoveReadPos(Int32 diff)
{
	m_read_pos += diff;
	if (m_read_pos < 0)
	{
		m_read_pos = 0;
	}

	if (m_read_pos > m_data_len)
	{
		m_read_pos = m_data_len;
	}
}

}