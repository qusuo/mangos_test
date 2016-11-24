#include "dbconnectionpool.h"
#include "dbconnection.h"
#include "../../../include/function_def.h"

//DBConnectionPool g_db_connect_pool;

DBConnectionPool::~DBConnectionPool()
{
	ConnectionList::iterator itr = busy_connections_.begin();
	ConnectionList::iterator itr_end = busy_connections_.end();
	for ( ; itr != itr_end; ++itr)
	{
		delete *itr;
	}

	itr = idle_connections_.begin();
	itr_end = idle_connections_.end();
	for ( ; itr != itr_end; ++itr)
	{
		delete *itr;
	}
}

bool DBConnectionPool::Init( const ConnectParams& connect_params, Int32 init_connection_num )
{
	connect_params_ = connect_params;
	if (init_connection_num > 0)
	{
		Int32 create_num = Min(MAX_CONNECTION_NUM, init_connection_num);
		Expand(init_connection_num);
	}
	return true;
}

DBConnection* DBConnectionPool::GetIdleConnection()
{
	if (idle_connections_.size() <= 0) return NULL;

	DBConnection* ret_connection = *(idle_connections_.begin());
	idle_connections_.pop_front();
	busy_connections_.push_back(ret_connection);
	return ret_connection;
}

void DBConnectionPool::BackToPool( DBConnection* connection )
{
	if (NULL == connection) return;

	ConnectionList::iterator itr = busy_connections_.begin();
	ConnectionList::iterator itr_end = busy_connections_.end();
	for ( ; itr != itr_end; ++itr)
	{
		if (*itr == connection)
		{
			busy_connections_.erase(itr);
			idle_connections_.push_back(connection);
			return;
		}
	}
}

void DBConnectionPool::Expand( Int32 to_add_num )
{
	Int32 cur_num = GetTotalConnectionNum();
	Int32 real_add_num = Min(to_add_num, MAX_CONNECTION_NUM - cur_num);

	for (Int32 i = 0; i < real_add_num; ++i)
	{
		CreateConnection();
	}
}

void DBConnectionPool::CreateConnection()
{
	MYSQL* mysql = mysql_init(NULL);
	if (NULL == mysql)
	{	
		printf("mysql init fail :%u, %s \n", mysql_errno(mysql), mysql_error(mysql));
		getchar();
		return;
	}

	MYSQL* connect_res = mysql_real_connect(mysql, 
											connect_params_.host.c_str(), 
											connect_params_.user.c_str(), 
											connect_params_.passwod.c_str(),
											connect_params_.database_name.c_str(),
											connect_params_.port,
											NULL,
											0);
	if (NULL == connect_res)
	{
		return;
	}
	mysql_set_character_set(mysql, "utf8");

	
	DBConnection* new_connection = new DBConnection(this, mysql);
	idle_connections_.push_back(new_connection);
	
}