#include "posi.h"
#include <math.h>

double PosiDistance(const Posi& lhs, const Posi rhs)
{
	double sum = pow(lhs.x - rhs.x, 2) + pow(lhs.y - rhs.y, 2);
	return sqrt(sum);
}
