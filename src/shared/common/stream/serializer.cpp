#include "serializer.h"
#include <memory>

namespace jxsstr
{ 

Serializer::Serializer()
	:m_data_buf(NULL)
	,m_buff_len(0)
	,m_write_pos(0)
	, m_is_auto_buff(false)
{
}

Serializer::~Serializer()
{
	ReleaseAutoBuff();
}

void Serializer::ReleaseAutoBuff()
{
	if (m_is_auto_buff && NULL != m_data_buf)
	{
		free(m_data_buf);
		m_data_buf = 0;
		m_is_auto_buff = false;
	}
}

Serializer::Serializer( char *buf, Int32 buf_len )
{
	m_data_buf = buf;
	m_buff_len = buf_len;
	m_write_pos = 0;
}

void Serializer::Reset( char *buf, Int32 buf_len )
{
	m_data_buf = buf;
	m_buff_len = buf_len;
	m_write_pos = 0;
}

bool Serializer::SetAutoBuff(Int32 default_buff_len)
{
	if (default_buff_len <= 0 || NULL != m_data_buf)
	{
		return false;
	}

	m_data_buf = (char*)malloc(default_buff_len);
	if (NULL == m_data_buf)
	{
		return false;
	}

	m_buff_len = default_buff_len;
	m_write_pos = 0;
	m_is_auto_buff = true;

	return true;
}

void Serializer::ExpandBuff()
{
	m_buff_len *= 2;
	m_data_buf = (char*)realloc(m_data_buf, m_buff_len);
}

bool Serializer::CheckExpand(Int32 req_new_len)
{
	const Int32 MAX_ROUND = 10;
	Int32 count = 0;
	while (count <= MAX_ROUND)
	{
		if (m_buff_len - m_write_pos >= req_new_len)
		{
			return true;
		}

		ExpandBuff();
		++count;
	}
	return false;
}

Int32 Serializer::GetUseLen()
{
	return m_write_pos;
}

char * Serializer::GetData()
{
	return m_data_buf;
}

bool Serializer::PutInt8( Int8 v )
{
	Int32 write_len = sizeof(Int8);
	if (m_is_auto_buff)
	{
		CheckExpand(write_len);
	}

	if (m_write_pos + write_len > m_buff_len)
	{
		return false;
	}
	else
	{
		memcpy(m_data_buf + m_write_pos, &v, write_len);
		m_write_pos += write_len;
		return true;
	}
}

bool Serializer::PutInt16( Int16 v )
{
	Int32 write_len = sizeof(v);
	if (m_is_auto_buff)
	{
		CheckExpand(write_len);
	}

	if (m_write_pos + write_len > m_buff_len)
	{
		return false;
	}
	else
	{
		memcpy(m_data_buf + m_write_pos, &v, write_len);
		m_write_pos += write_len;
		return true;
	}
}

bool Serializer::PutInt32( Int32 v )
{
	Int32 write_len = sizeof(v);
	if (m_is_auto_buff)
	{
		CheckExpand(write_len);
	}

	if (m_write_pos + write_len > m_buff_len)
	{
		return false;
	}
	else
	{
		memcpy(m_data_buf + m_write_pos, &v, write_len);
		m_write_pos += write_len;
		return true;
	}
}

bool Serializer::PutInt64( Int64 v )
{
	Int32 write_len = sizeof(v);
	if (m_is_auto_buff)
	{
		CheckExpand(write_len);
	}

	if (m_write_pos + write_len > m_buff_len)
	{
		return false;
	}
	else
	{
		memcpy(m_data_buf + m_write_pos, &v, write_len);
		m_write_pos += write_len;
		return true;
	}
}

bool Serializer::PutInt8s( Int8 *v, Int32 vl )
{
	Int32 write_len = sizeof(*v) * vl;
	if (m_is_auto_buff)
	{
		CheckExpand(write_len);
	}

	if (m_write_pos + write_len > m_buff_len)
	{
		return false;
	}
	else
	{
		memcpy(m_data_buf + m_write_pos, v, write_len);
		m_write_pos += write_len;
		return true;
	}
}

bool Serializer::PutChars(char* v, Int32 vl)
{
	Int32 write_len = sizeof(*v) * vl;
	if (m_is_auto_buff)
	{
		CheckExpand(write_len);
	}

	if (m_write_pos + write_len > m_buff_len)
	{
		return false;
	}
	else
	{
		memcpy(m_data_buf + m_write_pos, v, write_len);
		m_write_pos += write_len;
		return true;
	}
}

bool Serializer::PutInt16s( Int16 * v, Int32 vl )
{
	Int32 write_len = sizeof(*v) * vl;
	if (m_is_auto_buff)
	{
		CheckExpand(write_len);
	}

	if (m_write_pos + write_len > m_buff_len)
	{
		return false;
	}
	else
	{
		memcpy(m_data_buf + m_write_pos, v, write_len);
		m_write_pos += write_len;
		return true;
	}
}

bool Serializer::PutInt32s( Int32 * v, Int32 vl )
{
	Int32 write_len = sizeof(*v) * vl;
	if (m_is_auto_buff)
	{
		CheckExpand(write_len);
	}

	if (m_write_pos + write_len > m_buff_len)
	{
		return false;
	}
	else
	{
		memcpy(m_data_buf + m_write_pos, v, write_len);
		m_write_pos += write_len;
		return true;
	}
}

bool Serializer::PutInt64s( Int64 * v, Int32 vl )
{
	Int32 write_len = sizeof(*v) * vl;
	if (m_is_auto_buff)
	{
		CheckExpand(write_len);
	}

	if (m_write_pos + write_len > m_buff_len)
	{
		return false;
	}
	else
	{
		memcpy(m_data_buf + m_write_pos, v, write_len);
		m_write_pos += write_len;
		return true;
	}
}

bool Serializer::PutInt8Vector(Int8 * v, Int16 vl)
{
	if (!PutInt16(vl))
	{
		return false;
	}
	return PutInt8s(v, vl);
}

bool Serializer::PutInt16Vector(Int16 * v, Int16 vl)
{
	if (!PutInt16(vl))
	{
		return false;
	}
	return PutInt16s(v, vl);
}

bool Serializer::PutInt32Vector(Int32 * v, Int16 vl)
{
	if (!PutInt16(vl))
	{
		return false;
	}
	return PutInt32s(v, vl);
}

bool Serializer::PutInt64Vector(Int64 * v, Int16 vl)
{
	if (!PutInt16(vl))
	{
		return false;
	}
	return PutInt64s(v, vl);
}

}