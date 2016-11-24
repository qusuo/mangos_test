#include "config_api.h"
#include "../stream/strapi.h"
namespace configapi
{
	Int32 ParseToParamList(const std::string& src_str, char seperator, Int32 params[], Int32 max_num)
	{
		std::vector<std::string> param_strs;
		if (!jxsstr::SplitStr(src_str, seperator, param_strs))
		{
			return 0;
		}

		Int32 count = 0;
		for (uInt32 i = 0; i < param_strs.size() && count < max_num; ++i, ++count)
		{
			if (!jxsstr::AtoInt32(param_strs[i], params[count]))
			{
				continue;
			}
			//++count;
		}

		return count;
	}

	Int32 ParseToParamList(const std::string& src_str, Int32 types[], Int32 params[], Int32 max_num)
	{
		std::vector<std::string> param_strs;
		if (!jxsstr::SplitStr(src_str, '|', param_strs))
		{
			return 0;
		}

		Int32 count = 0;
		for (uInt32 i = 0; i < param_strs.size() && count < max_num; ++i)
		{
			std::vector<std::string> type_strs;
			if (!jxsstr::SplitStr(param_strs[i], ';', type_strs)
				|| type_strs.size() != 2)
			{
				continue;
			}
			if (!jxsstr::AtoInt32(type_strs[0], types[count])
				|| !jxsstr::AtoInt32(type_strs[1], params[count]))
			{
				continue;
			}
			++count;
		}

		return count;
	}

	Int32 ParseToParamListV1(const std::string& src_str, Int32 types[], Int32 params[], Int32 max_num)
	{
		std::vector<std::string> param_strs;
		if (!jxsstr::SplitStr(src_str, '|', param_strs))
		{
			return 0;
		}

		if (param_strs.size() < 2)
		{
			return false;
		}

		Int32 count = 0;
		for (uInt32 i = 0; i < param_strs.size() && count < max_num; ++i)
		{
			std::vector<std::string> type_strs;
			if (!jxsstr::SplitStr(param_strs[i], ';', type_strs)
				|| type_strs.size() < 2)
			{
				continue;
			}
			if (!jxsstr::AtoInt32(type_strs[0], types[count])
				|| !jxsstr::AtoInt32(type_strs[1], params[count]))
			{
				continue;
			}
			++count;
		}

		return count;
	}

	Int32 ParseToParamList(const std::string& src_str, Int32 types[], double params[], Int32 max_num)
	{
		std::vector<std::string> param_strs;
		if (!jxsstr::SplitStr(src_str, '|', param_strs))
		{
			return 0;
		}

		Int32 count = 0;
		for (uInt32 i = 0; i < param_strs.size() && count < max_num; ++i, ++count)
		{
			std::vector<std::string> type_strs;
			if (!jxsstr::SplitStr(param_strs[i], ';', type_strs) || type_strs.size() < 2)
			{
				continue;
			}
			if (!jxsstr::AtoInt32(type_strs[0], types[count])
				|| !jxsstr::AtoDouble(type_strs[1], params[count]))
			{
				continue;
			}
			//++count;
		}

		return count;
	}

	Int32 ParseToParamList(const std::string& src_str, Int32List params[], Int32 max_num)
	{
		std::vector<std::string> param_strs;
		if (!jxsstr::SplitStr(src_str, '|', param_strs))
		{
			return 0;
		}

		Int32 count = 0;
		for (uInt32 i = 0; i < param_strs.size() && count < max_num; ++i)
		{
			std::vector<std::string> type_strs;
			if (!jxsstr::SplitStr(param_strs[i], ';', type_strs))
			{
				continue;
			}
			Int32List& tar_list = params[count];
			for (uInt32 j = 0; j < type_strs.size(); ++j)
			{
				Int32 tmp_param = 0;
				if (jxsstr::AtoInt32(type_strs[j], tmp_param))
				{
					tar_list.push_back(tmp_param);
				}
			}
			++count;
		}

		return count;
	}

	Int64 ParseFormatTime(const std::string& src_str)
	{
		Int32 str_size = (Int32)src_str.size();
		char buffer[16] = { 0 };
		Int32 time_int[6] = { 0 };
		Int32 buffer_index = 0;
		Int32 time_index = 0;
		std::stringstream ss;

		for (Int32 i = 0; i < str_size; ++i)
		{
			assert(time_index <= 6);
			assert(buffer_index <= 16);
			if ('/' == src_str[i] || ':' == src_str[i] || i == str_size - 1)
			{
				ss.clear();
				ss.str("");
				ss << buffer;
				ss >> time_int[time_index];
				++time_index;
				memset(buffer, 0, sizeof(buffer));
				buffer_index = 0;
			}
			else
			{
				buffer[buffer_index] = src_str[i];
				++buffer_index;
			}
		}

		//取得从1970/01/01到目标日期的秒数
		struct tm t;
		t.tm_year = time_int[0] - 1900;
		t.tm_mon = time_int[1] - 1;
		t.tm_mday = time_int[2];
		t.tm_hour = time_int[3];
		t.tm_min = time_int[4];
		t.tm_sec = time_int[5];

		return mktime(&t);
	}
}