#include "strapi.h"
#include "../include/header_def.h"
#include <sstream>
#include <stdlib.h>


namespace jxsstr
{
	char base64_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

	bool Base64Encode( const char * src, int src_len, char * out_buff, int& out_len )
	{
		//MIME规定Base64编码每行长度为76字节
		out_len = 0;
		char tmp_buff[4];
		int line_len = 0;
		for(int i = 0; i < src_len; i += 3)
		{
			memset(tmp_buff, 0, 4);

			tmp_buff[0] = src[i];
			tmp_buff[1] = src[i+1];
			tmp_buff[2] = src[i+2];

			int len = (int)strlen(tmp_buff);
			if(3 == len)
			{
				out_buff[out_len++] = base64_table[((int)tmp_buff[0] & 0xFC)>>2];
				out_buff[out_len++] = base64_table[((int)tmp_buff[0] & 0x3)<<4 | ((int)tmp_buff[1] & 0xF0)>>4];
				out_buff[out_len++] = base64_table[((int)tmp_buff[1] & 0xF)<<2 | ((int)tmp_buff[2] & 0xC0)>>6];
				out_buff[out_len++] = base64_table[(int)tmp_buff[2] & 0x3F];
				line_len += 4;
				if(line_len >= 76) 
				{
					out_buff[out_len++] = '\r';
					out_buff[out_len++] = '\n';
					line_len=0;
				}
			}
			else if(len == 2)
			{
				out_buff[out_len++] = base64_table[((int)tmp_buff[0] & 0xFC)>>2];
				out_buff[out_len++] = base64_table[((int)tmp_buff[0] & 0x3 )<<4 | ((int)tmp_buff[1] & 0xF0 )>>4];
				out_buff[out_len++] = base64_table[((int)tmp_buff[1] & 0x0F)<<2];
				out_buff[out_len++] = '=';
				line_len += 4;
				if(line_len >= 76) 
				{
					out_buff[out_len++] = '\r';
					out_buff[out_len++] = '\n';
					line_len=0;
				}
			}
			else if(len==1)
			{
				out_buff[out_len++] = base64_table[((int)tmp_buff[0] & 0xFC)>>2];
				out_buff[out_len++] = base64_table[((int)tmp_buff[0] & 0x3 )<<4];
				out_buff[out_len++] = '=';
				out_buff[out_len++] = '=';
				line_len += 4;
				if(line_len >= 76) 
				{
					out_buff[out_len++] = '\r';
					out_buff[out_len++] = '\n';
					line_len=0;
				}
			}
			memset(tmp_buff, 0, 4);
		}
		return true;
	}

	bool Base64Decode( const char * src, int src_len, char * out_buff, int& out_len )
	{
		static int table_len = (int)strlen(base64_table);
		out_len = 0;
		char tmp_buff[5];
		int len = 0;
		for(int i = 0 ;i < src_len ;i += 4)
		{
			memset(tmp_buff, 0, 5);
			tmp_buff[0] = src[i];
			tmp_buff[1] = src[i+1];
			tmp_buff[2] = src[i+2];
			tmp_buff[3] = src[i+3];

			len+=4;
			if(76 == len)
			{
				i += 2;
				len = 0;
			}

			int asc[4] = { 0 };
			for(int j = 0 ;j < 4 ; j++)
			{
				for(int k = 0 ;k< table_len; k++)
				{
					if(tmp_buff[j] == base64_table[k]) asc[j] = k;
				}
			}

			if('=' == tmp_buff[2] && '=' == tmp_buff[3])
			{
				out_buff[out_len++] = (char)(int)(asc[0] << 2 | asc[1] << 2 >> 6);
			}
			else if('=' == tmp_buff[3])
			{
				out_buff[out_len++] = (char)(int)(asc[0] << 2 | asc[1] << 2 >> 6);
				out_buff[out_len++] = (char)(int)(asc[1] << 4 | asc[2] << 2 >> 4);
			}
			else
			{
				out_buff[out_len++] = (char)(int)(asc[0] << 2 | asc[1] << 2 >> 6);
				out_buff[out_len++] = (char)(int)(asc[1] << 4 | asc[2] << 2 >> 4);
				out_buff[out_len++] = (char)(int)(asc[2] << 6 | asc[3] << 2 >> 2);
			}  
		}
		return true;
	}

	bool SplitStr( const std::string& src, char separator, std::vector<std::string>& out_substrs )
	{
		std::string tmp_sub;
		for (size_t i = 0;i < src.size(); i++)
		{
			if (src[i] == separator)
			{
				out_substrs.push_back(tmp_sub);
				tmp_sub.clear();
			}
			else
			{
				tmp_sub += src[i];
			}
		}
		out_substrs.push_back(tmp_sub);
		return true;
	}

	int ReplaceStr(std::string& src, const std::string& be_replaced_str, const std::string& replace_str)
	{
		std::string::size_type pos = 0;
		std::string::size_type a = be_replaced_str.size();
		std::string::size_type b = replace_str.size();
		int count = 0;
		while ((pos = src.find(be_replaced_str, pos)) != std::string::npos)
		{
			src.replace(pos, a, replace_str);
			pos += b;
			++count;
		}
		return count;
	}

	bool AtoInt32(std::string str, Int32& out_int)
	{
		std::stringstream ss(str);
		ss >> out_int;
		return true;
	}

	bool AtoInt64(std::string str, Int64& out_int)
	{
		std::stringstream ss(str);
		ss >> out_int;
		return true;
	}

	bool AtoDouble(std::string str, double& out_double)
	{
		std::stringstream ss(str);
		ss >> out_double;
		return true;
	}

	std::string IntToA(Int32 num)
	{
		std::stringstream ss;
		ss << num;
		return ss.str();
	}

	size_t strlcpy(char *dst, const char *src, size_t dsize)
	{
		if (!dst || !src)
			return -1;

		const char *osrc = src;
		size_t nleft = dsize;

		if (nleft != 0) {
			while (--nleft != 0) {
				if ((*dst++ = *src++) == '\0')
					break;
			}
		}

		if (nleft == 0) {
			if (dsize != 0)
				*dst = '\0';
			while (*src++)
				;
		}

		return(src - osrc - 1);
	}

}//end namespace strapi