#ifndef JXS_JSONAPI_H_
#define JXS_JSONAPI_H_
//author:hsj
//对jsoncpp做一些封装
#include "json.h"
#include "../../include/basic_type_def.h"

namespace jxsjson
{
	bool LoadFromFile(const char* file_dir, Json::Value& out_root, std::string& out_err);
	bool SaveToFile(const char* file_dir, Json::Value& root_value, std::string& out_err);
	bool GetSubKeyValue(Json::Value& parent, const char* key, Json::Value& out_value);
	bool GetSubKeyValue(Json::Value& parent, const char* key, Int32& out_value, bool is_null_as_zero = true);
	bool GetSubKeyValue(Json::Value& parent, const char* key, Int64& out_value, bool is_null_as_zero = true);
	bool GetSubKeyValue(Json::Value& parent, const char* key, Int16& out_value, bool is_null_as_zero = true);
	bool GetSubKeyValue(Json::Value& parent, const char* key, double& out_value, bool is_null_as_zero = true);
	bool GetSubKeyValue(Json::Value& parent, const char* key, std::string& out_value, bool is_null_as_empty = true);
	bool GetSubKeyValue(Json::Value& parent, const char* key, char out_value[], Int32 buff_len);
}


#endif