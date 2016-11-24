#include "day_id.h"
#include "../bit/bit_api.h"
#include "time_api.h"

namespace jxstime
{
	const Int32 LEAP_YEAR_DAY_NUM = 366;
	const Int32 NONLEAP_YEAR_DAY_NUM = 365;
	inline bool IsLeapYear(Int32 year)
	{
		return (year % 100 != 0) && (year % 4 == 0);
	}
	bool IsLeagleDayOfYear(Int32 year, Int32 day)
	{
		if (year < 0 || day < 0) return false;

		if (IsLeapYear(year))
		{
			if (day < LEAP_YEAR_DAY_NUM) return true;
		}
		else
		{
			if (day < NONLEAP_YEAR_DAY_NUM) return true;
		}

		return false;
	}

	Int32 DayID::ToInt32() const
	{
		return jxsbit::Pack32(m_year, m_day_of_year);
	}

	DayID::DayID( Int32 int32_day_id )
	{
		m_year = jxsbit::Hight16(int32_day_id);
		m_day_of_year = jxsbit::Low16(int32_day_id);
	}

	DayID::DayID(Int64 time_stamp)
	{
		Int16 year;
		Int16 yday;
		time_t cur_time = time_stamp > 0 ? time_stamp : PISTime();
#ifdef _WIN32
		tm lt;
		localtime_s(&lt, &cur_time);
		year = lt.tm_year + 1900;
		yday = lt.tm_yday;
#else
		struct tm* lt = localtime(&cur_time);
		year = lt->tm_year + 1900;
		yday = lt->tm_yday;
#endif
		m_year = year;
		m_day_of_year = yday;
	}

	DayID DayID::operator+( Int32 day_num ) const
	{
		Int16 year = m_year;
		Int16 day  = m_day_of_year;
		Int32 sum_day = day + day_num;

		while(!IsLeagleDayOfYear(year, sum_day))
		{
			if (year < 0) return DayID();

			if (sum_day > 0)
			{
				if (IsLeapYear(year))
				{
					sum_day -= LEAP_YEAR_DAY_NUM;
				}
				else
				{
					sum_day -= NONLEAP_YEAR_DAY_NUM;
				}
				++year;
			}
			else
			{
				if (IsLeapYear(year - 1))
				{
					sum_day += LEAP_YEAR_DAY_NUM;
				}
				else
				{
					sum_day += NONLEAP_YEAR_DAY_NUM;
				}
				--year;
			}
		}

		return DayID(year, sum_day);
	}

	DayID DayID::operator-( Int32 day_num ) const
	{
		return (*this + (-day_num));
	}
	Int32 operator-( const DayID& lhs, const DayID& rhs )
	{
		Int16 year_lhs	= lhs.GetYear();
		Int16 day_lhs	= lhs.GetDayOfYear();
		Int16 year_rhs	= rhs.GetYear();
		Int16 day_rhs	= rhs.GetDayOfYear();

		Int16 year_diff	= year_lhs - year_rhs;
		if (year_diff == 0)
		{
			return day_lhs - day_rhs;
		}
		else
		{
			Int32 ret_day = 0;
			bool reverse_flag	= false;
			if (year_diff < 0 )
			{
				Int16 tmp		= year_lhs;
				year_lhs		= year_rhs;
				year_rhs		= tmp;
				tmp				= day_lhs;
				day_lhs			= day_rhs;
				day_rhs			= day_lhs;
				reverse_flag	= true;
			}

			bool leap_year = IsLeapYear(year_rhs); 
			if (leap_year)
			{
				ret_day += LEAP_YEAR_DAY_NUM - day_rhs - 1;//-1 因为dayid 从0开始
			}
			else
			{
				ret_day += NONLEAP_YEAR_DAY_NUM - day_rhs - 1;
			}

			for (Int16 i = year_rhs+1; i < year_lhs; ++i)
			{
				leap_year = (i % 4 == 0 ? true : false);
				if (leap_year)
				{
					ret_day += LEAP_YEAR_DAY_NUM;
				}
				else
				{
					ret_day += NONLEAP_YEAR_DAY_NUM;
				}
			}

			ret_day += day_lhs + 1;//+1 因为dayid从0开始

			return reverse_flag ? -ret_day : ret_day;
		}
	}

}