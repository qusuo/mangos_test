#include "dbconnection.h"
#include "../../../include/header_def.h"
#include "../../../include/value_def.h"
#include <string>

DBConnection::~DBConnection()
{
	Close();
}

DBConnection::DBConnection( DBConnectionPool* owner, MYSQL* mysql )
	:owner_(owner)
	,mysql_(mysql)
	, m_st(NULL)
{
}

//DBQueryResult* DBConnection::ExecuteQuery(const char *format, ...)
//{
//	query_result_.Close();
//
//	char buffer[1000 * 4] = { 0 };
//	va_list args;
//	va_start(args, format);
//	vsnprintf(buffer, sizeof(buffer) - 1, format, args);
//	va_end(args);
//
//	Int32 query_res = mysql_real_query(mysql_, buffer, strlen(buffer));
//	const Int32 QUERY_RES_SUCC = 0;
//	if (QUERY_RES_SUCC == query_res)
//	{
//		MYSQL_RES* mysql_res = mysql_store_result(mysql_);
//		query_result_.ResetResult(mysql_res);
//		return &query_result_;
//	}
//	else
//	{
//		return NULL;
//	}
//}

DBQueryResult* DBConnection::ExecuteQuery(const char *sql_str)
{
	CloseStmt();

	query_result_.Close();

	Int32 query_res = mysql_real_query(mysql_, sql_str, strlen(sql_str));
	const Int32 QUERY_RES_SUCC = 0;
	if (QUERY_RES_SUCC == query_res)
	{
		MYSQL_RES* mysql_res = mysql_store_result(mysql_);
		query_result_.ResetResult(mysql_res);
		return &query_result_;
	}
	else
	{
		printf("execute sql error res:%d \n", query_res);
		return NULL;
	}
}

Int32 DBConnection::ExecuteCommand( const char* statement )
{
	if (NULL == mysql_ || NULL == statement) return 0;
	query_result_.Close();

	Int32 query_res = mysql_real_query(mysql_, statement, strlen(statement));
	const Int32 QUERY_RES_SUCC = 0;
	if (QUERY_RES_SUCC == query_res)
	{
		Int32 affected_row_num = mysql_affected_rows(mysql_);
		return affected_row_num;
	}
	else
	{
		return 0;
	}
}

void DBConnection::Close()
{
	if (NULL != m_st)
	{
		mysql_stmt_close(m_st);
		m_st = NULL;
	}

	if (NULL != mysql_)
	{
		mysql_close(mysql_);
		mysql_ = NULL;
	}
}

const char* DBConnection::Escape(const char* str, Int32 str_len)
{
	const Int32 MAX_ESCAPE_DATA_LEN = MAX_1M_BUFFER_LEN;
	if (mysql_ == NULL || str == NULL || str_len <= 0)
	{
		return NULL;
	}
	if (str_len >= MAX_ESCAPE_DATA_LEN)
	{
		return NULL;
	}

	static char buff[MAX_ESCAPE_DATA_LEN];
	memset(buff, 0, sizeof(buff));

	mysql_real_escape_string(mysql_, buff, (char*)str, str_len);

	return buff;
}

void DBConnection::Ping()
{
	mysql_ping(mysql_);
}

Int32 DBConnection::StmtStart(const char* str, Int32 str_len)
{
	InitStmt();
	return mysql_stmt_prepare(m_st, str, str_len);
}

Int32 DBConnection::StmtBindParam(MYSQL_BIND* bind)
{
	return mysql_stmt_bind_param(m_st, bind);
}

DbStmtResult* DBConnection::StmtExecute(MYSQL_BIND* res)
{
	Int32 query_res = mysql_stmt_execute(m_st);
	const Int32 QUERY_RES_SUCC = 0;
	if (QUERY_RES_SUCC == query_res)
	{	
		MYSQL_RES* mysql_res = mysql_stmt_result_metadata(m_st);

		if (NULL != res)
		{
			mysql_stmt_bind_result(m_st, res);
			mysql_stmt_store_result(m_st);
		}
		m_stmt_res.ResetResult(mysql_res, m_st);
		return &m_stmt_res;
	}
	else
	{
		printf("execute sql error res:%s \n", mysql_stmt_error(m_st));
		return NULL;
	}
}

const char* DBConnection::StmtError()
{
	if (NULL != m_st)
	{
		return mysql_stmt_error(m_st);
	}
	else
	{
		return "null m_stmt";
	}
}

void DBConnection::InitStmt()
{
	if (NULL != m_st)
	{
		CloseStmt();
	}
	m_st = mysql_stmt_init(mysql_);
}

void DBConnection::CloseStmt()
{
	if (NULL != m_st)
	{
		mysql_stmt_free_result(m_st);
		mysql_stmt_close(m_st);		
		m_st = NULL;
	}
}


