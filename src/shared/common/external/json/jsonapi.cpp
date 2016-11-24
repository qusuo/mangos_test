#include "jsonapi.h"
namespace jxsjson
{
	bool LoadFromFile(const char* file_dir, Json::Value& out_root, std::string& out_err)
	{
		FILE *file = fopen(file_dir, "rb");
		if (NULL == file)
		{
			out_err = "open file error!";
			return false;
		}
		fseek(file, 0, SEEK_END);
		long size = ftell(file);
		fseek(file, 0, SEEK_SET);
		std::string text;
		char *buffer = new char[size + 1];
		buffer[size] = 0;
		if (fread(buffer, 1, size, file) == (unsigned long)size)
		{
			text = buffer;
		}		
		fclose(file);
		delete[] buffer;	

		Json::Features features;
		Json::Reader reader(features);

		bool parse_res = reader.parse(text, out_root);
		if (!parse_res)
		{
			out_err = reader.getFormatedErrorMessages();
			return false;
		}

		return true;
	}

	bool SaveToFile(const char* file_dir, Json::Value& root_value, std::string& out_err)
	{
		FILE *file = fopen(file_dir, "w");
		if (NULL == file)
		{
			out_err = "open file error!";
			return false;
		}
		
		fprintf(file, root_value.toStyledString().c_str());
		fclose(file);
		return true;
	}

	bool GetSubKeyValue(Json::Value& parent, const char* key, Json::Value& out_value)
	{
		out_value = parent[key];
		return true;			
	}

	bool GetSubKeyValue(Json::Value& parent, const char* key, Int32& out_value, bool is_null_as_zero)
	{
		Json::Value tmp_value = parent[key];

		if (tmp_value.isInt())
		{
			out_value = tmp_value.asInt();
			return true;
		}
		else if (is_null_as_zero && tmp_value.isNull())
		{
			out_value = 0;
			return true;
			
		}
		else
		{
			return false;
		}
	}

	bool GetSubKeyValue(Json::Value& parent, const char* key, Int64& out_value, bool is_null_as_zero)
	{
		Json::Value tmp_value = parent[key];

		if (tmp_value.isInt())
		{
			out_value = tmp_value.asInt();
			return true;
		}
		else if (is_null_as_zero && tmp_value.isNull())
		{
			out_value = 0;
			return true;

		}
		else
		{
			return false;
		}
	}

	bool GetSubKeyValue(Json::Value& parent, const char* key, Int16& out_value, bool is_null_as_zero)
	{
		Json::Value tmp_value = parent[key];

		if (tmp_value.isInt())
		{
			out_value = tmp_value.asInt();
			return true;
		}
		else if (is_null_as_zero && tmp_value.isNull())
		{
			out_value = 0;
			return true;

		}
		else
		{
			return false;
		}
	}

	bool GetSubKeyValue(Json::Value& parent, const char* key, double& out_value, bool is_null_as_zero)
	{
		Json::Value tmp_value = parent[key];

		if (tmp_value.isDouble())
		{
			out_value = tmp_value.asDouble();
			return true;
		}
		else if (is_null_as_zero && tmp_value.isNull())
		{
			out_value = 0;
			return true;

		}
		else
		{
			return false;
		}
	}

	bool GetSubKeyValue(Json::Value& parent, const char* key, std::string& out_value, bool is_null_as_empty)
	{
		Json::Value tmp_value = parent[key];

		if (tmp_value.isNull())
		{
			out_value = "";
			return true;
		}
		else if (tmp_value.isString())
		{
			out_value = tmp_value.asString();
			return true;
		}
		else if (tmp_value.isInt())
		{
			std::stringstream ss;
			std::string str;
			ss << tmp_value.asInt();
			ss >> out_value;
			return true;
		}
		else
		{
			return false;
		}
	}

	bool GetSubKeyValue(Json::Value& parent, const char* key, char out_value[], Int32 buff_len)
	{
		Json::Value tmp_value = parent[key];

		if (tmp_value.isString())
		{
			if (tmp_value.asString().size() >= buff_len)
			{
				return false;
			}
			else
			{
				strcpy(out_value, tmp_value.asString().c_str());
				return true;
			}

		}
		else if (tmp_value.isInt())
		{
			std::stringstream ss;
			ss << tmp_value.asInt();
			if (ss.str().size() >= buff_len)
			{
				return false;
			}
			else
			{
				ss >> out_value;
				return true;
			}
		}
		else
		{
			return false;
		}
	}

}