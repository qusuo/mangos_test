#include "game_config_loader.h"
#include "buffer_config.h"
#include "card_config.h"
#include "equip_config.h"
#include "hero_config.h"
#include "monster_config.h"
#include "skill_config.h"
#include "strategy_config.h"
#include "ai_config.h"

#include <string>
#include <assert.h>

namespace gameconfig
{
	bool LoadConfig(const char* path_dir)
	{
		std::string tmp_path = path_dir;
		std::string file_dir;

		printf("loading config path:%s \n", path_dir);

		file_dir = tmp_path + "/" + "card_config.json";
		bool res = g_card_cfg_mgr.Init(file_dir.c_str());
		assert(res);

		file_dir = tmp_path + "/" + "buffer_config.json";
		res = g_buffer_cfg_mgr.Init(file_dir.c_str());
		assert(res);

		file_dir = tmp_path + "/" + "skill_config.json";
		res = g_skill_cfg_mgr.Init(file_dir.c_str());
		assert(res);

		file_dir = tmp_path + "/" + "effect_config.json";
		res = g_skill_cfg_mgr.InitEffect(file_dir.c_str());
		assert(res);

		file_dir = tmp_path + "/" + "monster_config.json";
		res = g_monster_cfg_mgr.Init(file_dir.c_str());
		assert(res);

		file_dir = tmp_path + "/" + "hero_config.json";
		res = g_hero_cfg_mgr.Init(file_dir.c_str());
		assert(res);



		file_dir = tmp_path + "/" + "strategy_config.json";
		res = g_strategy_config_mgr.Init(file_dir.c_str());
		assert(res);

		file_dir = tmp_path + "/" + "ai_config.json";
		res = g_ai_config_mgr.Init(file_dir.c_str());
		assert(res);

		return res;
	}

}