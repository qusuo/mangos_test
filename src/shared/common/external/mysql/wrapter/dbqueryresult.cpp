#include "dbqueryresult.h"
#include "../../../include/function_def.h"

DBQueryResult::DBQueryResult()
	:mysql_res_(NULL)
	,current_row_()
{

}

DBQueryResult::~DBQueryResult()
{
	Close();
}

void DBQueryResult::ResetResult( MYSQL_RES* mysql_res )
{
	if (NULL == mysql_res) return;
	Close();

	mysql_res_ = mysql_res;
	current_row_ = mysql_fetch_row(mysql_res_);
	Int32 field_num = GetFieldNum();
	MYSQL_FIELD* fields = mysql_fetch_fields(mysql_res_);
	for (Int32 i = 0; i < field_num; ++i)//结果集的第0行保存的就是字段名
	{
		field_name_index_map_[fields[i].name] = i;
	}
}

Int32 DBQueryResult::GetRowNum() const
{
	if (NULL != mysql_res_)
	{
		return mysql_num_rows(mysql_res_);
	}
	else
	{
		return 0;
	}
}

Int32 DBQueryResult::GetFieldNum() const
{
	if (NULL != mysql_res_)
	{
		return mysql_num_fields(mysql_res_);
	}
	else
	{
		return 0;
	}
}

bool DBQueryResult::SeekRow( uInt32 row_index )
{
	if (NULL == mysql_res_) return false;
	Int32 row_num = GetRowNum();
	if (Int32(row_index) < row_num)
	{
		mysql_data_seek(mysql_res_, row_index);
		current_row_ = mysql_fetch_row(mysql_res_);
		return true;
	}
	else
	{
		return false;
	}
}

bool DBQueryResult::IsValidFieldIndex(uInt32 field_index)
{
	if (NULL == mysql_res_) return false;

	Int32 field_num = GetFieldNum();
	if (Int32(field_index) < field_num)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void DBQueryResult::Close()
{
	if (NULL != mysql_res_)
	{
		mysql_free_result(mysql_res_);
		mysql_res_ = NULL;
		current_row_ = NULL;
		field_name_index_map_.clear();
	}
}

void DBQueryResult::ToNextRow()
{
	if (NULL == mysql_res_) return;
	current_row_ = mysql_fetch_row(mysql_res_);
}

bool DBQueryResult::GetField( Int32 field_index, std::string& out_field_value )
{
	if (NULL == mysql_res_) return false;
	if (!IsValidFieldIndex(field_index)) return false;

	out_field_value = current_row_[field_index] == NULL ? "" : current_row_[field_index];

	return true;
}

bool DBQueryResult::GetField( Int32 field_index, char* out_field_value )
{
	if (NULL == mysql_res_) return false;
	if (!IsValidFieldIndex(field_index)) return false;

	const char* field_value = current_row_[field_index] == NULL ? "" : current_row_[field_index];

	strcpy(out_field_value, field_value);
	return true;
}

bool DBQueryResult::GetBlobField(Int32 field_index, Int32 max_field_len, char* out_field_data, Int32& out_data_len)
{
	if (NULL == mysql_res_) return false;
	if (!IsValidFieldIndex(field_index)) return false;
	unsigned long *lengths = mysql_fetch_lengths(mysql_res_);

	Int32 data_len = Min(lengths[field_index], max_field_len);
	const char* field_value = current_row_[field_index] == NULL ? "" : current_row_[field_index];
	memcpy(out_field_data, field_value, data_len);
	out_data_len = data_len;
	return true;
}
