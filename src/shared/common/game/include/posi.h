#ifndef JXS_POSI_H_
#define JXS_POSI_H_
//author:hsj
#include "../../include/basic_type_def.h"

struct Posi
{
	Posi() :x(-1), y(-1){}
	Posi(Int16 _x, Int16 _y) :x(_x), y(_y){}

	Posi operator-(const Posi &right) const
	{
		return Posi(x - right.x, y - right.y);
	}

	Posi operator+(const Posi &right) const
	{
		return Posi(x + right.x, y + right.y);
	}

	bool operator==(const Posi &right) const
	{
		return (x == right.x && y == right.y);
	}

	bool operator!=(const Posi &right) const
	{
		return !operator==(right);
	}

	bool operator<(const Posi &right) const
	{
		return (x < right.x || (x == right.x && y < right.y));
	}

	bool IsValidPos()
	{
		return x >= 0 && y >= 0;
	}

	Int16 x;
	Int16 y;
};

double PosiDistance(const Posi& lhs, const Posi rhs);


#endif