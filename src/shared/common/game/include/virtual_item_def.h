#ifndef JXS_VIRTUALITEMDEF_H_
#define JXS_VIRTUALITEMDEF_H_
//author:hsj
//虚拟物品id定义

enum VirtualItem
{
	VIRTUAL_ITEM_INVALID,
	VIRTUAL_ITEM_TAEL = 1,				//银两
	VIRTUAL_ITEM_FORAGE = 2,			//粮草
	VIRTUAL_ITEM_WOOD = 3,				//木头
	VIRTUAL_ITEM_COPPER = 4,			//铜锭
	VIRTUAL_ITEM_INGOT = 5,				//铁锭
	VIRTUAL_ITEM_ALL_GOLD = 6,			//元宝
	VIRTUAL_ITEM_ROLE_LEVEL = 7,		//角色等级
	VIRTUAL_ITEM_ROLE_EXP = 8,			//角色经验
	VIRTUAL_ITEM_MILITARY_RANK = 9,		//军衔
	VIRTUAL_ITEM_REPUATION = 10,		//声望
	VIRTUAL_ITEM_ENERGY = 11,			//能量
	VIRTUAL_ITEM_LEADERSHIP = 12,		//统率
	VIRTUAL_ITEM_BOOM = 13,				//繁荣
	VIRTUAL_ITEM_GOLD = 14,				//充值元宝
	VIRTUAL_ITEM_BIND_GOLD = 15,		//普通元宝
	VIRTUAL_ITEM_VIP_LEVEL = 16,		//vip等级

	VIRTUAL_ITEM_TAEL_YIELDS = 19,		//银两产量
	VIRTUAL_ITEM_FORAGE_YIELDS,			//粮草产量
	VIRTUAL_ITEM_WOOD_YIELDS,			//木材产量
	VIRTUAL_ITEM_COPPER_YIELDS,			//铜锭产量
	VIRTUAL_ITEM_INGOT_YIELDS,			//铁锭产量
	VIRTUAL_ITEM_TOTAL_YIELDS,			//资源总产量
	

	//add before
	VIRTUAL_ITEM_PROC_ACHIEVE,		//等待进度完成，test, 将会取消
	VIRTUAL_ITEM_END,
};

//军团建设度	17
//军团个人贡献	18



#endif