#ifndef NOC_COMMON_STREAM_DESERIALIZER_H_
#define NOC_COMMON_STREAM_DESERIALIZER_H_
//author:hsj
//序列化工具
#include "../include/basic_type_def.h"

namespace jxsstr
{
class Deserializer
{
public:
	Deserializer();
	Deserializer(char *buf, Int32 data_len);
	void Reset(char *buf, Int32 data_len);

	void MoveReadPos(Int32 diff);

	Int32 GetDataLen();
	char* GetData();
	Int32 ViewInt32();

	bool ViewInt8(Int8& v);
	bool ViewInt16(Int16& v);	
	bool ViewInt32(Int32& v);
	bool ViewInt32(Int32 index, Int32& v);

	bool GetInt8(Int8& v);
	bool GetInt16(Int16& v);
	bool GetInt32(Int32& v);
	bool GetInt64(Int64 &v);

	//定长数组
	bool GetInt8s(Int8 * v, Int32 vl);//定长,保存int8数组
	bool GetInt16s(Int16 * v, Int32 vl);//定长,保存int16数组
	bool GetInt32s(Int32 * v, Int32 vl);//定长,保存int32数组
	bool GetInt64s(Int64 * v, Int32 vl);//定长,保存int32数组

	//变长数组 注意长度不要超16位整数的最大值
	bool GetInt8Vector(Int8 * v, Int16 max_len, Int16& out_len);//变长,保存int8数组
	bool GetInt16Vector(Int16 * v, Int32 max_len, Int16& out_len);//变长,保存int16数组
	bool GetInt32Vector(Int32 * v, Int32 max_len, Int16& out_len);//变长,保存int32数组
	bool GetInt64Vector(Int64 * v, Int32 max_len, Int16& out_len);//变长,保存int32数组

protected:
	char* m_data_buf;
	Int32 m_data_len;
	Int32 m_read_pos;

};
}
#endif