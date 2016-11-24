#ifndef JXS_GAMEBATTLE_H_
#define JXS_GAMEBATTLE_H_
//author:hsj
//战斗库外部接口
//#include "../include/server_def.h"
#include "../metadata/battle_meta.h"

class ProtHandler
{
public:
	ProtHandler(){}
	virtual ~ProtHandler(){}
	virtual void SendBattleProt(RoleID role_id, Int64 battle_id, const char* data, Int32 data_len) = 0;

private:

};

namespace jxslibbattle
{
	bool InitLibBattle(ProtHandler* handler, const char* config_path, const char* log_path);
	void Update(Int64 cur_time);
	Int64 StartBattle(BattleMeta& battle_meta, Int32 a_ai_id, Int32 b_ai_id);
	void UserInput(RoleID role_id, Int64 battle_id, const char* data, Int32 data_len);
	void SystemInput(Int64 battle_id, const char* data, Int32 data_len);
}

#endif