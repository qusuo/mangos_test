#ifndef NOC_DBSERVER_RDBMS_DBQUERYRESULT_H_
#define NOC_DBSERVER_RDBMS_DBQUERYRESULT_H_
//数据库查询结果集
//msyql中，每一个连接只对应一个结果集，所以每次查询之前，都要释放前一个查询的结果集
#include "../../../include/basic_type_def.h"
#include "../../../include/header_def.h"
#include <mysql.h>
#include <string>
#include <map>
#include <sstream>

class DBQueryResult
{
	friend class DBConnection;
public:
	DBQueryResult();
	~DBQueryResult();
	void ResetResult(MYSQL_RES* mysql_res);
	Int32 GetRowNum() const;
	Int32 GetFieldNum() const;
	bool SeekRow(uInt32 row_index);
	void ToNextRow();

	template <class T>
	bool GetField(const char* field_name, T& out_field_value);
	template <class T>
	bool GetField(Int32 field_index, T& out_field_value);

	bool GetField(Int32 field_index, std::string& out_field_value);
	bool GetField(Int32 field_index, char* out_field_value);
	bool GetBlobField(Int32 field_index, Int32 max_field_len, char* out_field_data, Int32& out_data_len);

private:
	void Close();
	bool IsValidFieldIndex(uInt32 field_index);

private:
	MYSQL_RES* mysql_res_;
	MYSQL_ROW current_row_;

	//<name, index>
	typedef std::map<std::string, Int32> FieldNameIndexMap;
	FieldNameIndexMap field_name_index_map_;
};

template <class T>
bool DBQueryResult::GetField( const char* field_name, T& out_field_value )
{
	if (NULL == field_name) return false;
	FieldNameIndexMap::iterator itr = field_name_index_map_.find(field_name);
	if (itr == field_name_index_map_.end()) return false;
	Int32 field_index = itr->second;

	return GetField(field_index, out_field_value);
}

template <class T>
bool DBQueryResult::GetField( Int32 field_index, T& out_field_value )
{
	if (NULL == mysql_res_) return false;
	if (!IsValidFieldIndex(field_index)) return false;

	char* field_value = current_row_[field_index];
	if (NULL != field_value)
	{
		std::stringstream ss;
		ss << field_value;
		ss >> out_field_value;
	}

	return true;
}


#endif