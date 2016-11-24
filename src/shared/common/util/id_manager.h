#ifndef JXS_IDMANAGER_H_
#define JXS_IDMANAGER_H_
//author:hsj
//可回收id 的管理器
#include <vector>
#include "../include/basic_type_def.h"

namespace jxsutil
{
class IDManager
{
public:
	IDManager();
	~IDManager();

	bool Init(Int32 id_num);

	Int32 GetOutID();
	void PutBackID(Int32 id);

private:
	void AddIDs(Int32 add_num);

private:
	typedef std::vector<Int32> IDArray;
	IDArray m_avail_ids;
	Int32 m_next_gen_id;
};
}//end namespace basiclib

#endif