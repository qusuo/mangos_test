#ifndef NOC_COMMON_STREAM_SERIALIZER_H_
#define NOC_COMMON_STREAM_SERIALIZER_H_
//author:hsj
//序列化工具
#include "../include/basic_type_def.h"

namespace jxsstr
{ 

class Serializer
{
public:
	Serializer();
	~Serializer();
	Serializer(char *buf, Int32 buf_len);
	void Reset(char *buf, Int32 buf_len);
	bool SetAutoBuff(Int32 default_buff_len);
	void ReleaseAutoBuff();

	Int32 GetUseLen();
	char *GetData();

	bool PutInt8(Int8 v);
	bool PutInt16(Int16 v);
	bool PutInt32(Int32 v);
	bool PutInt64(Int64 v);

	bool PutChars(char* v, Int32 vl);

	//定长数组
	bool PutInt8s(Int8 *v, Int32 vl);	//定长,保存int8数组
	bool PutInt16s(Int16 * v, Int32 vl);//定长,保存int16数组
	bool PutInt32s(Int32 * v, Int32 vl);//定长,保存int32数组
	bool PutInt64s(Int64 * v, Int32 vl);//定长,保存int32数组
	
	//变长数组
	bool PutInt8Vector(Int8 * v, Int16 vl);
	bool PutInt16Vector(Int16 * v, Int16 vl);
	bool PutInt32Vector(Int32 * v, Int16 vl);
	bool PutInt64Vector(Int64 * v, Int16 vl);

	void IncWritePos(Int32 len){ m_write_pos += len; }

private:
	bool CheckExpand(Int32 req_new_len);
	void ExpandBuff();

private:
	char* m_data_buf;
	Int32 m_buff_len;
	Int32 m_write_pos;
	bool m_is_auto_buff;
};

}
#endif