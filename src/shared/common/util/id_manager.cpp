#include "id_manager.h"
#include "../include/function_def.h"
#include "../include/value_def.h"

namespace jxsutil
{
	const Int32 DEFAULT_INIT_ID = 500;
	const Int32 MAX_INT_ID = 1000000;
	IDManager::IDManager()
		:m_next_gen_id(1)
	{	
	}

	bool IDManager::Init(Int32 id_num)
	{
		if (id_num <= 0)
		{
			id_num = DEFAULT_INIT_ID;
		}

		AddIDs(id_num);
		return true;
	}


	IDManager::~IDManager()
	{

	}

	Int32 IDManager::GetOutID()
	{	
		if (m_next_gen_id >= MAX_INT_ID)
		{
			return 0;
		}

		Int32 avail_id_num = (Int32)m_avail_ids.size();
		if (avail_id_num <= 0)
		{
			Int32 add_num = Min(MAX_INT32_NUMBER - m_next_gen_id, DEFAULT_INIT_ID);
			AddIDs(add_num);
		}

		avail_id_num = (Int32)m_avail_ids.size();
		if (avail_id_num <= 0)
		{
			return 0;
		}

		Int32 ret_id = m_avail_ids[avail_id_num-1];
		m_avail_ids.pop_back();
		return ret_id;
	}

	void IDManager::AddIDs( Int32 add_num )
	{
		//m_avail_ids.resize(m_next_gen_id + add_num);
		Int32 count = 0;
		for (Int32 i = m_next_gen_id; count < add_num; ++i, ++count)
		{
			m_avail_ids.push_back(i);
		}
		m_next_gen_id += add_num;
	}

	void IDManager::PutBackID( Int32 id )
	{
		m_avail_ids.push_back(id);
	}

}//end namespace basiclib