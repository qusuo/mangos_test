#ifndef JXS_GAMECONFIGAPI_H_
#define JXS_GAMECONFIGAPI_H_

#include "../structs/item_data.h"

namespace gameconfigapi
{
	//格式id;num|id;num   空或"0"表示没有道具
	Int32 ParseItemParams(const std::string& str, ItemParam* params, Int32 max_num);
}


#endif