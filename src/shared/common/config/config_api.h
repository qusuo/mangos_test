#ifndef JXS_CONFIGAPI_H_
#define JXS_CONFIGAPI_H_

#include "../include/basic_type_def.h"
#include "../external/json/jsonapi.h"
typedef std::vector<Int32> Int32List;

//不小于0
#define JSON_GET_SUBNODE_INT_VALUE(file_path, parent_node, node_name, out_value) \
	if (!jxsjson::GetSubKeyValue(parent_node, node_name, out_value)\
	)\
	{\
		printf("LoadFromFile[%s] error, %s:%d \n", file_path, node_name, out_value);\
		return false;\
	}

#define JSON_GET_SUBNODE_INT_VALUE_L(file_path, parent_node, node_name, out_value, limit_left) \
	if (!jxsjson::GetSubKeyValue(parent_node, node_name, out_value)\
		|| out_value <= limit_left \
	)\
	{\
		printf("LoadFromFile[%s] error, %s:%d \n", file_path, node_name, out_value);\
		return false;\
	}

#define JSON_GET_SUBNODE_INT_VALUE_LR(file_path, parent_node, node_name, out_value, limit_left, limit_right) \
	if (!jxsjson::GetSubKeyValue(parent_node, node_name, out_value)\
	|| out_value <= limit_left \
	|| out_value >= limit_right \
	)\
	{\
		printf("LoadFromFile[%s] error, %s:%d \n", file_path, node_name, out_value);\
		return false;\
	}

namespace configapi
{
	//单元之间用|分隔， 参数之间用;分隔
	Int32 ParseToParamList(const std::string& src_str, char seperator, Int32 params[], Int32 max_num);
	Int32 ParseToParamList(const std::string& src_str, Int32 types[], Int32 params[], Int32 max_num);
	Int32 ParseToParamListV1(const std::string& src_str, Int32 types[], Int32 params[], Int32 max_num);
	Int32 ParseToParamList(const std::string& src_str, Int32 types[], double params[], Int32 max_num);
	Int32 ParseToParamList(const std::string& src_str, Int32List params[], Int32 max_num);
	//time string format: YYYY/MM/DD/HH:MM:SS
	Int64 ParseFormatTime(const std::string& src_str);
}

#endif