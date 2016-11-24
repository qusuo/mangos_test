#include "server_net_config.h"
#include "../../external/json/jsonapi.h"

ServerNetConfig::ServerNetConfig()
{

}

ServerNetConfig::~ServerNetConfig()
{

}

ServerNetConfig& ServerNetConfig::Singleton()
{
	static ServerNetConfig instance;
	return instance;
}

bool ParseNetParam(Json::Value jvalue, NetListenParam& net_param)
{
	if (!jxsjson::GetSubKeyValue(jvalue, "ip", net_param.ip)
		|| !jxsjson::GetSubKeyValue(jvalue, "port", net_param.port)
		|| net_param.port <= 0)
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool ServerNetConfig::Init(const char* file_path)
{
	if (!LoadFromFile(file_path))
	{
		return false;
	}
	else
	{
		m_file_path = file_path;
		return true;
	}
}

bool ServerNetConfig::LoadFromFile(const char* file_path)
{
	Json::Value root;
	std::string err;
	bool res = jxsjson::LoadFromFile(file_path, root, err);
	if (!res)
	{
		printf("LoadFromFile[%s] error: %s \n", file_path, err.c_str());
		return false;
	}

	Json::Value tmp_value;
	NetListenParam tmp_param;

	if (!jxsjson::GetSubKeyValue(root, "log_server", tmp_value)
		|| !ParseNetParam(tmp_value, tmp_param))
	{
		printf("LoadFromFile[%s]  log_server read error  code %s \n", file_path, err.c_str());
		return false;
	}
	m_log_server_net = tmp_param;

	if (!jxsjson::GetSubKeyValue(root, "data_server", tmp_value)
		|| !ParseNetParam(tmp_value, tmp_param))
	{
		printf("LoadFromFile[%s]  data_server read error  code %s \n", file_path, err.c_str());
		return false;
	}
	m_data_server_net = tmp_param;

	Json::Value login_value;
	if (!jxsjson::GetSubKeyValue(root, "login_server", login_value))
	{
		printf("LoadFromFile[%s]  login_server read error  code %s \n", file_path, err.c_str());
		return false;
	}
	if (!jxsjson::GetSubKeyValue(login_value, "for_client", tmp_value)
		|| !ParseNetParam(tmp_value, tmp_param))
	{
		printf("LoadFromFile[%s]  data_server read error  code %s \n", file_path, err.c_str());
		return false;
	}
	m_login_client_net = tmp_param;
	if (!jxsjson::GetSubKeyValue(login_value, "for_server", tmp_value)
		|| !ParseNetParam(tmp_value, tmp_param))
	{
		printf("LoadFromFile[%s]  data_server read error  code %s \n", file_path, err.c_str());
		return false;
	}
	m_login_server_net = tmp_param;

	if (!jxsjson::GetSubKeyValue(root, "global_server", tmp_value)
		|| !ParseNetParam(tmp_value, tmp_param))
	{
		printf("LoadFromFile[%s]  global_server read error  code %s \n", file_path, err.c_str());
		return false;
	}
	m_global_server_net = tmp_param;

	if (!jxsjson::GetSubKeyValue(root, "gamelogic_server", tmp_value)
		|| !ParseNetParam(tmp_value, tmp_param))
	{
		printf("LoadFromFile[%s]  gamelogic_server read error  code %s \n", file_path, err.c_str());
		return false;
	}
	m_gamelogic_server_net = tmp_param;

	Json::Value gateway_value;
	if (!jxsjson::GetSubKeyValue(root, "gateway_server", gateway_value))
	{
		printf("LoadFromFile[%s]  gateway_server read error  code %s \n", file_path, err.c_str());
		return false;
	}

	for (uInt32 i = 0; i < gateway_value.size(); ++i)
	{
		tmp_value = gateway_value[i];
		if (!ParseNetParam(tmp_value, tmp_param))
		{
			printf("LoadFromFile[%s]  gateway_server->net_list index:%d read error  code %s \n", file_path, i, err.c_str());
			return false;
		}
		m_gateway_server_net.push_back(tmp_param);
	}

	Json::Value gm_value;
	if (!jxsjson::GetSubKeyValue(root, "gm_server", gm_value))
	{
		printf("LoadFromFile[%s]  gm_server read error  code %s \n", file_path, err.c_str());
		return false;
	}
	if (!jxsjson::GetSubKeyValue(gm_value, "for_client", tmp_value)
		|| !ParseNetParam(tmp_value, tmp_param))
	{
		printf("LoadFromFile[%s]  gm_server read error  code %s \n", file_path, err.c_str());
		return false;
	}
	m_gm_client_net = tmp_param;
	if (!jxsjson::GetSubKeyValue(gm_value, "for_server", tmp_value)
		|| !ParseNetParam(tmp_value, tmp_param))
	{
		printf("LoadFromFile[%s]  gm_server read error  code %s \n", file_path, err.c_str());
		return false;
	}
	m_gm_server_net = tmp_param;

	if (!jxsjson::GetSubKeyValue(root, "out_ip", m_outer_ip))
	{
		printf("LoadFromFile[%s]  out_ip read error  code %s \n", file_path, err.c_str());
		return false;
	}

	return true;
}

bool ServerNetConfig::PreLoad()
{
	m_log_server_net_bak = m_log_server_net;
	m_data_server_net_bak = m_data_server_net;
	m_login_server_net_bak = m_login_server_net;
	m_login_client_net = m_login_client_net;
	m_global_server_net_bak = m_global_server_net;
	m_gamelogic_server_net_bak = m_gamelogic_server_net;
	m_gateway_server_net_bak = m_gateway_server_net;
	m_gm_client_net_bak = m_gm_client_net;
	m_gm_server_net_bak = m_gm_server_net;

	return LoadFromFile(m_file_path.c_str());
}

bool ServerNetConfig::ConformPreLoad()
{
	m_gateway_server_net.clear();
	return true;
}

bool ServerNetConfig::RollBackPreLoad()
{
	m_log_server_net = m_log_server_net_bak;
	m_data_server_net = m_data_server_net_bak;
	m_login_server_net = m_login_server_net_bak;
	m_login_client_net = m_login_client_net_bak;
	m_global_server_net = m_global_server_net_bak;
	m_gamelogic_server_net = m_gamelogic_server_net_bak;
	m_gateway_server_net = m_gateway_server_net_bak;
	m_gm_client_net = m_gm_client_net_bak;
	m_gm_server_net = m_gm_server_net_bak;
	return true;
}

NetListenParam* ServerNetConfig::GetLogSeverToServer()
{
	return &m_log_server_net;
}

NetListenParam* ServerNetConfig::GetDataSeverToServer()
{
	return &m_data_server_net;
}

NetListenParam* ServerNetConfig::GetLoginServerToServer()
{
	return &m_login_server_net;
}

NetListenParam* ServerNetConfig::GetLoginServerToClient()
{
	return &m_login_client_net;
}

NetListenParam* ServerNetConfig::GetGlobalSeverToServer()
{
	return &m_global_server_net;
}

NetListenParam* ServerNetConfig::GetGameLogicServerToServer()
{
	return &m_gamelogic_server_net;
}

NetListenParam* ServerNetConfig::GetGatewayServerToClient(uInt32 node_index)
{
	if (node_index >= m_gateway_server_net.size())
	{
		return NULL;
	}
	else
	{
		return &m_gateway_server_net[node_index];
	}
}

NetListenParam* ServerNetConfig::GetGmServerToServer()
{
	return &m_gm_server_net;
}

NetListenParam* ServerNetConfig::GetGmServerToClient()
{
	return &m_gm_client_net;
}


