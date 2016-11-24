#ifndef JXS_ALLIANCEMEMBERSTRUCT_H_
#define JXS_ALLIANCEMEMBERSTRUCT_H_

#include "../../include/game_value_def.h"
#include "../../include/data_len_def.h"
#include "../../../stream/serializer.h"
#include "../../../stream/deserializer.h"

struct AllianceMemberStruct
{
	AllianceMemberStruct()
		: role_id(0)
		, alliance_id(0)
		, post(0)
		, logic_data_len(0)
		{}

	~AllianceMemberStruct(){}

	bool Serialize(jxsstr::Serializer& se);
	bool Deserialize(jxsstr::Deserializer& ds);
	void Reset();

	RoleID role_id;
	Int64 alliance_id;
	Int16 post;							//职位 1:军团长 0:普通成员


	/** format:
			join_time
			week_donate
			week_contribution
			cur_contribution
			day_welfare_state
			m_active_schedule
	*/
	Int32 logic_data_len;
	static const Int32 ALLIANCE_MEMBER_LOGIC_DATA_LEN1 = 64;
	Int8 logic_data[ALLIANCE_MEMBER_LOGIC_DATA_LEN1];
};

#endif