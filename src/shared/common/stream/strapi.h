#ifndef NOC_COMMON_STREAM_STRAPI_H_
#define NOC_COMMON_STREAM_STRAPI_H_
//author:hsj
//字符串处理工具接口

#include <string>
#include <vector>
#include "../include/basic_type_def.h"

namespace jxsstr
{
	// 编码后的长度一般比原文多占1/3的存储空间，请保证base64code有足够的空间
	bool Base64Encode( const char * src, int src_len, char * out_buff, int& out_len );
	bool Base64Decode( const char * src, int src_len, char * out_buff, int& out_len );
	bool SplitStr(const std::string& src, char separator, std::vector<std::string>& out_substrs);
	int ReplaceStr(std::string& src, const std::string& be_replaced_str, const std::string& replace_str);
	bool AtoInt32(std::string str, Int32& out_int);
	bool AtoInt64(std::string str, Int64& out_int);
	bool AtoDouble(std::string str, double& out_int);
	std::string IntToA(Int32 num);
	size_t strlcpy(char *dst, const char *src, size_t dsize); // 返回实际拷贝的长度

	std::wstring C2W(const char *pc);
	std::string W2C(const wchar_t * pw);
}

#endif