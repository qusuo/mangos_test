#ifndef JXS_BITAPI_H_
#define JXS_BITAPI_H_
//author: hsj

#include "../include/basic_type_def.h"

namespace jxsbit
{
	Int8 Hight8(Int16 val);	//高8位
	Int8 Low8(Int16 val);	//低8位
	Int16 Hight16(Int32 val);	//高16位
	Int16 Low16(Int32 val);	//低16位
	Int32 Hight32(Int64 val);	//高32位
	Int32 Low32(Int64 val);	//低32位
	Int32 Pack16(char hight_val, char low_val);	//两个字节合成一个16位整数
	Int32 Pack32(Int16 hight_val, Int16 low_val);	//两个整数合成一个32位整数
	Int64 Pack64(Int32 hight_val, Int32 low_val);	//两个整数合成一个64位整数
}
#endif