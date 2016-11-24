#include "mathapi.h"
#include <math.h>
#include "../include/function_def.h"
#include <stdlib.h>

namespace jxsmath
{
	Int32 Range(Int32 val, Int32 min, Int32 max)
	{
		val = Max(val, min);
		val = Min(val, max);
		return val;
	}

	Int32 Floor(double val)	//向下取整
	{
		return (Int32)floor(val);
	}

	Int32 Ceil(double val)	//向上取整
	{
		return (Int32)ceil(val);
	}

	Int32 Round(double val)	//四舍五入
	{
		return (Int32)(val + 0.5);
	}

	//取min和max区间内的随机数 包括min和max
	uInt32 Random(uInt32 min, uInt32 max)
	{
		if (min > max)	//min 必须小于等于 max
		{
			return 0;
		}

		double rnd = (double)(rand() % RAND_MAX) / (double)RAND_MAX;
		uInt32 res = (uInt32)(rnd * (max - min + 1)) + min;
		return res;
	}

	Int32 SignedRandom(Int32 min, Int32 max)
	{
		if (min > max)	//min 必须小于等于 max
		{
			return 0;
		}

		double rnd = (double)(rand() % RAND_MAX) / (double)RAND_MAX;
		Int32 res = rnd * (max - min + 1) + min;
		return res;
	}

	//在min和max区间内，判断是否命中rate_tar概率
	bool HitRate(uInt32 rate_tar, uInt32 min, uInt32 max)
	{
		uInt32 rnd = Random(min, max);
		if (rnd >= min && rnd <= rate_tar)
		{
			return true;
		}
		return false;
	}

	bool HitRange(Int32 range_list[], Int32 range_num, Int32& out_pos)
	{
		Int32 total_range = 0;
		for (Int32 i = 0; i < range_num; ++i)
		{
			total_range += range_list[i];
		}
		
		Int32 rnd = Random(0, total_range - 1);
		for (Int32 i = 0; i < range_num; ++i)
		{
			if (rnd < range_list[i])
			{
				out_pos = i;
				return true;
			}
			else
			{
				rnd -= range_list[i];
			}
		}

		return false;
	}

	bool HitRandomVal(Int32 random_val, Int32 min, Int32 max)
	{
		if (random_val >= min && random_val <= max)
		{
			return true;
		}
		return false;
	}

	void SetBit(char* data, uInt32 index, uInt32 set_value)
	{
		uInt32 byte_index = index / 8;
		uInt32 byte_offset = 8 - (index) % 8 - 1;

		uInt8 flag = 0x1 << byte_offset;
		if (0 == set_value)
		{
			flag = ~flag;
			data[byte_index] = data[byte_index] & flag;
		}
		else
		{
			data[byte_index] = data[byte_index] | flag;
		}
	}

	uInt32 GetBit(char* data, uInt32 index)
	{
		uInt32 byte_index = index / 8;
		uInt32 byte_offset = 8 - (index) % 8 - 1;
		uInt8 flag = 0x1 << byte_offset;

		uInt32 ret = data[byte_index] & flag;
		return ret > 0 ? 1 : 0;
	}

	//每两个位的值  index 是第几个值
	void SetDBit(char* data, unsigned int index, unsigned int set_value)
	{
		unsigned byte_index = index * 2 / 8;
		unsigned byte_offset = 8 - index * 2 % 8 - 2 ;

		unsigned char flag = set_value > 3 ? 3 : set_value;
		flag = flag << byte_offset;
		unsigned char help_flag = 0x3 << byte_offset;
		help_flag = ~help_flag;
		data[byte_index] = data[byte_index] & help_flag;
		data[byte_index] = data[byte_index] | flag;
	}

	//每两个位的值
	unsigned GetDBit(const char* data, unsigned int index)
	{
		unsigned byte_index = index * 2 / 8;
		unsigned byte_offset = 8 - index * 2 % 8 - 2 ;
		unsigned char flag = 0x3 << byte_offset;

		unsigned char ret = data[byte_index] & flag;
		ret = ret >> byte_offset;
		ret = ret & 0x3;
		return ret;
	}

	//#define MIN_INT32_NUMBER Int32((uInt32)1 << 31);
	//检查溢出加法，如果溢出，则保留最大（小）值
	//Int32 CheckOverflowPlus(Int32 a, Int32 b)
	//{
	//	Int32 ret = a + b;
	//	if (ret < 0 && a > 0 && b > 0)
	//	{
	//		return MAX_INT32_NUMBER;
	//	}
	//	else if (ret > 0 && a < 0 && b < 0)
	//	{
	//		return MIN_INT32_NUMBER;
	//	}
	//	else
	//	{
	//		return ret;
	//	}
	//}

	//从区间中随机抽取get_num个不重复的数, get_num不能大于RANDOM_MAX_GET_NUM
	//const Int32 RANDOM_MAX_GET_NUM = 100;
	bool Random(Int32 min, Int32 max, Int32 get_num, Int32* res_out)
	{
		Int32 diff = max - min + 1;
		if (get_num <= 0 || diff <= 0 || diff < get_num || get_num > RANDOM_MAX_GET_NUM || NULL == res_out)
		{
			return false;
		}
		static bool flags[RANDOM_MAX_GET_NUM];

		memset(flags, 0, sizeof(flags));
		Int32 res_num = 0;
		for (Int32 i = 0; i < get_num; ++i)
		{
			double rnd = (double)(rand() % RAND_MAX) / (double)RAND_MAX;
			Int32 res = (Int32)(rnd * (diff - res_num));//用剩下的个数随机出来一个

			while(flags[res])//找到下一个未被选出来的
			{
				++res;
				if (res >= diff) res = 0;
			}

			flags[res]			= true;
			res_out[res_num]	= min + res;
			++res_num;
		}

		return true;
	}

	double UniformDistribution(double min_value, double max_value)
	{
		double rand_value = (double)(rand() / (double)RAND_MAX);
		rand_value = rand_value * (max_value - min_value) + min_value;
		return rand_value;
	}

	double NormalDistribution(double e, double v)
	{
		static double V1, V2, S;
		static int phase = 0;
		double X;

		if (phase == 0) {
			do {
				double U1 = (double)rand() / RAND_MAX;
				double U2 = (double)rand() / RAND_MAX;

				V1 = 2 * U1 - 1;
				V2 = 2 * U2 - 1;
				S = V1 * V1 + V2 * V2;
			} while (S >= 1 || S == 0);

			X = V1 * sqrt(-2 * log(S) / S);
		}
		else
			X = V2 * sqrt(-2 * log(S) / S);

		phase = 1 - phase;

		return X * v + e;
	}

}