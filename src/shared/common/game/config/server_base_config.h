#ifndef JXS_SERVERBASECONFIG_H_
#define JXS_SERVERBASECONFIG_H_

//服务器配置
#include "../../config/config-interface.h"
#include "../../include/value_def.h"
#include "../../include/basic_type_def.h"


class ServerBaseConfig : public jxs::Config
{
public:
	virtual ~ServerBaseConfig();
	virtual bool Init(const char* file_path);
	virtual bool PreLoad();
	virtual bool ConformPreLoad();
	virtual bool RollBackPreLoad();
	static ServerBaseConfig& Singleton();

	Int64 GetOpenServerTime() { return m_open_server_time; }
	Int32 GetOpenServerEscapeDay();	//获取开服了多长时间，单位天
	Int32 GetServerId() { return m_server_id; }
	Int32 GetPlatId() { return m_plat_id; }

	bool IsAntiAddictionOpened() { return 1 == m_is_anti_addiction_opened; }
	Int32 GetAntiAddictionCutTimeLen() { return m_anti_addiction_cut_time_len; }
	Int32 GetAntiAddictionOutTimeLen() { return m_anti_addiction_out_time_len; }
	void SetAntiAddctionOpen(Int32 is_open) { m_is_anti_addiction_opened = is_open; }
	void SetAntiAddctionCutTimeLen(Int32 time_len) { m_anti_addiction_cut_time_len = time_len; }
	void SetAntiAddctionOutTimeLen(Int32 time_len) { m_anti_addiction_out_time_len = time_len; }

private:
	ServerBaseConfig();

	bool LoadFromFile(const char* file_path);

private:
	std::string m_file_path;
	//
	Int64 m_open_server_time;
	Int32 m_server_id;
	Int32 m_plat_id;
	Int32 m_is_anti_addiction_opened;		//是否开放防沉迷
	Int32 m_anti_addiction_cut_time_len;	//收益减半时间
	Int32 m_anti_addiction_out_time_len;	//踢人时间


	//bak
	Int64 m_open_server_time_bak;
	Int32 m_server_id_bak;
	Int32 m_plat_id_bak;
	Int32 m_is_anti_addiction_opened_bak;		//是否开放防沉迷
	Int32 m_anti_addiction_cut_time_len_bak;	//收益减半时间
	Int32 m_anti_addiction_out_time_len_bak;	//踢人时间
};

#define g_server_cfg ServerBaseConfig::Singleton()

#endif