#ifndef JXS_DAYID_H_
#define JXS_DAYID_H_
#include "../include/basic_type_def.h"
//author: hsj

namespace jxstime
{
class DayID
{
public:
	DayID():m_year(0), m_day_of_year(0){}
	DayID(Int16 year, Int16 year_of_day):m_year(year), m_day_of_year(year_of_day){}
	~DayID(){}
	explicit DayID(Int32 int32_day_id);
	DayID(const DayID& day_id) { m_year = day_id.m_year; m_day_of_year = day_id.m_day_of_year; }
	explicit DayID(Int64 time_stamp);

	bool operator==(const DayID& rhs) const { return m_year == rhs.m_year && m_day_of_year == rhs.m_day_of_year; }
	bool operator!=(const DayID& rhs) const { return m_year != rhs.m_year || m_day_of_year != rhs.m_day_of_year; }
	bool operator<(const DayID& rhs) const
	{
		return m_year < rhs.m_year || (m_year == rhs.m_year && m_day_of_year < rhs.m_day_of_year);
	}
	DayID operator+(Int32 day_num) const;
	DayID operator-(Int32 day_num) const;

	Int32 ToInt32() const;
	Int32 GetYear() const { return m_year; }
	Int32 GetDayOfYear() const { return m_day_of_year; }

private:
	Int16 m_year;
	Int16 m_day_of_year;

};

//return diff day num
Int32 operator-(const DayID& lhs, const DayID& rhs);

}

#endif