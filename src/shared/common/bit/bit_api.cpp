#include "bit_api.h"

namespace jxsbit
{
	Int8 Hight8(Int16 val)	//高8位
	{
		return (char)(val >> 8);
	}
	Int8 Low8(Int16 val)	//低8位
	{
		return (char)(val & 0xFF);
	}

	Int16 Hight16(Int32 val)	//高16位
	{
		return (Int16)(val >> 16);
	}
	Int16 Low16(Int32 val)	//低16位
	{
		return (Int16)(val & 0xFFFF);
	}

	Int32 Hight16(Int64 val)	//高32位
	{
		return (Int32)(val >> 32);
	}

	Int32 Low16(Int64 val)	//低32位
	{
		return (Int32)(val & 0xFFFFFFFF);
	}

	Int32 Pack16(char hight_val, char low_val)	//两个字节合成一个16位整数
	{
		return ((Int16)hight_val << 8 | low_val);
	}

	Int32 Pack32(Int16 hight_val, Int16 low_val)	//两个整数合成一个32位整数
	{
		return ((Int32)hight_val << 16 | low_val);
	}

	Int64 Pack64(Int32 hight_val, Int32 low_val)	//两个整数合成一个64位整数
	{
		return ((Int64)hight_val << 32 | low_val);
	}
}