#ifndef JXS_SERVERNETCONFIG_H_
#define JXS_SERVERNETCONFIG_H_

//服务器配置
#include "../../config/config-interface.h"
#include "../../include/value_def.h"
#include "../../include/basic_type_def.h"

struct NetListenParam
{
	NetListenParam()
		:port(0)
	{
	}
	std::string ip;
	Int32 port;
};

class ServerNetConfig : public jxs::Config
{
public:	
	virtual ~ServerNetConfig();
	virtual bool Init(const char* file_path);
	virtual bool PreLoad();
	virtual bool ConformPreLoad();
	virtual bool RollBackPreLoad();
	static ServerNetConfig& Singleton();

	NetListenParam* GetLogSeverToServer();
	NetListenParam* GetDataSeverToServer();
	NetListenParam* GetLoginServerToServer();
	NetListenParam* GetLoginServerToClient();
	NetListenParam* GetGlobalSeverToServer();	
	NetListenParam* GetGameLogicServerToServer();
	NetListenParam* GetGatewayServerToClient(uInt32 node_index);
	NetListenParam* GetGmServerToServer();
	NetListenParam* GetGmServerToClient();
	const char* GetOuterIp() { return m_outer_ip.c_str(); }
private:
	ServerNetConfig();

	bool LoadFromFile(const char* file_path);

	typedef std::vector<NetListenParam> NetParamList;
private:
	std::string m_file_path;
	NetListenParam m_log_server_net;
	NetListenParam m_data_server_net;	
	NetListenParam m_login_server_net;
	NetListenParam m_login_client_net;
	NetListenParam m_global_server_net;
	NetListenParam m_gamelogic_server_net;	
	NetParamList m_gateway_server_net;
	NetListenParam m_gm_server_net;
	NetListenParam m_gm_client_net;

	//bak
	NetListenParam m_log_server_net_bak;
	NetListenParam m_data_server_net_bak;	
	NetListenParam m_login_server_net_bak;
	NetListenParam m_login_client_net_bak;
	NetListenParam m_global_server_net_bak;
	NetListenParam m_gamelogic_server_net_bak;
	NetParamList m_gateway_server_net_bak;
	NetListenParam m_gm_server_net_bak;
	NetListenParam m_gm_client_net_bak;

	std::string m_outer_ip;
};

#define g_net_config ServerNetConfig::Singleton()

#endif