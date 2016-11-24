#ifndef NOC_DBSERVER_LOGIC_RDBMS_DBCONNECTIONPOOL_H_
#define NOC_DBSERVER_LOGIC_RDBMS_DBCONNECTIONPOOL_H_
//数据库连接池
//multi thread isn't support

#include "../../../include/basic_type_def.h"
#include <list>
#include <string>

struct ConnectParams
{
	ConnectParams()
		:port(0)
	{

	}
	std::string host;
	Int32 port;
	std::string database_name;
	std::string user;
	std::string passwod;	
};

class DBConnection;
class DBConnectionPool
{
public:
	~DBConnectionPool();
	bool Init(const ConnectParams& connect_params, Int32 init_connection_num);
	Int32 GetTotalConnectionNum() const	{ return busy_connections_.size() + idle_connections_.size(); }
	DBConnection* GetIdleConnection();
	void BackToPool(DBConnection* connection);
	void Expand(Int32 to_add_num);

private:
	void CreateConnection();


	static const Int32 MAX_CONNECTION_NUM = 500;
private:
	typedef std::list<DBConnection*> ConnectionList;
	ConnectionList busy_connections_;
	ConnectionList idle_connections_;
	ConnectParams connect_params_;
};

//extern DBConnectionPool g_db_connect_pool;

#endif