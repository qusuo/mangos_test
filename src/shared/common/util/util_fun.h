#ifndef JXS_UTILFUN_H_
#define JXS_UTILFUN_H_
//常用工具函数
#include "../include/basic_type_def.h"

namespace jxsutil
{
template<class T>
void IntMemSet(T* dest, T val, Int32 num)
{
	for (Int32 i = 0; i < num; ++i)
	{
		dest[i] = val;
	}
}

// 返回实际拷贝的长度
size_t strlcpy(char *dst, const char *src, size_t dsize);

}

#endif