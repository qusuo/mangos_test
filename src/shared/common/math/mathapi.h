#ifndef JXS_MATHAPI_H_
#define JXS_MATHAPI_H_
//author:hsj

#include "../include/basic_type_def.h"

namespace jxsmath
{

	Int32 Range(Int32 val, Int32 min, Int32 max);
	Int32 Floor(double val);	//向下取整
	Int32 Ceil(double val);	//向上取整
	Int32 Round(double val);	//四舍五入

	//取min和max区间内的随机数 包括min和max
	uInt32 Random(uInt32 min, uInt32 max);

	Int32 SignedRandom(Int32 min, Int32 max);

	//在min和max区间内，判断是否命中rate_tar概率
	bool HitRate(uInt32 rate_tar, uInt32 min, uInt32 max);
	//根据每个区间的份额，算计出命中哪个区间
	bool HitRange(Int32 range_list[], Int32 range_num, Int32& out_pos);

	bool HitRandomVal(Int32 random_val, Int32 min, Int32 max);
	void SetBit(char* data, uInt32 index, uInt32 set_value);
	uInt32 GetBit(char* data, uInt32 index);

	//每两个位的值  index 是第几个值
	void SetDBit(char* data, unsigned int index, unsigned int set_value);

	//每两个位的值
	unsigned GetDBit(const char* data, unsigned int index);

	#define MIN_INT32_NUMBER Int32((uInt32)1 << 31);
	//检查溢出加法，如果溢出，则保留最大（小）值
	Int32 CheckOverflowPlus(Int32 a, Int32 b);

	//从区间中随机抽取get_num个不重复的数, get_num不能大于RANDOM_MAX_GET_NUM
	const Int32 RANDOM_MAX_GET_NUM = 100;
	bool Random(Int32 min, Int32 max, Int32 get_num, Int32* res_out);

	//随机分布函数
	//连续均匀分布
	double UniformDistribution(double min_value, double max_value);
	//高斯/正态分布（gaussian/normal distribution）e:期望，v方差
	double NormalDistribution(double e, double v);



}


#endif