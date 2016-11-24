#include "game_config_api.h"
#include "../../config/config_api.h"
#include "../../stream/strapi.h"
#include "../../log/logapi.h"

namespace gameconfigapi
{
	Int32 ParseItemParams(const std::string& str, ItemParam* params, Int32 max_num)
	{
		if (str.length() <= 0 
			|| (str.length() == 1 && str[0] == '0')
			|| max_num <= 0)
		{
			return 0;
		}
		
		Int32List* param_list = new Int32List[max_num];
		Int32 res = configapi::ParseToParamList(str, param_list, max_num);

		for (Int32 i = 0; i < res; ++i)
		{
			if (param_list[i].size() != 2)
			{
				DEBUG_LOG("param_list[i].size():%d != 2", param_list[i].size());
				break;
			}
			params[i].item_id = param_list[i][0];
			params[i].item_num = param_list[i][1];
		}

		delete [] param_list;
		return res;
	}

}