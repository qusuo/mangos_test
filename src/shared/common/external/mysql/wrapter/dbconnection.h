#ifndef NOC_DBSEVER_RDBMS_DBCONNECTION_H_
#define NOC_DBSEVER_RDBMS_DBCONNECTION_H_
//数据库连接
#include "dbqueryresult.h"
#include "db_stmt_result.h"

class DBConnectionPool;
class DBConnection
{
public:
	~DBConnection();
	DBConnection(DBConnectionPool* owner, MYSQL* mysql);
	//DBQueryResult* ExecuteQuery(const char *format, ...);

	//
	DBQueryResult* ExecuteQuery(const char *sql_str);
	Int32 ExecuteCommand(const char* statement);
	DBQueryResult* GetQueryResult() { return &query_result_; }
	DBConnectionPool* GetOwner() { return owner_; }
	const char* Escape(const char* str, Int32 str_len);
	const char* Error();
	void Ping();

	//stmt相关
	Int32 StmtStart(const char* str, Int32 str_len);
	Int32 StmtBindParam(MYSQL_BIND* bind);
	DbStmtResult* StmtExecute(MYSQL_BIND* res);
	const char* StmtError();

protected:
	void Close();
	void InitStmt();
	void CloseStmt();

private:
	DBConnectionPool* owner_;	
	MYSQL* mysql_;
	DBQueryResult query_result_;
	MYSQL_STMT * m_st;
	DbStmtResult m_stmt_res;
};
#endif