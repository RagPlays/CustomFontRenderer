#include "CollisionUtils.h"

bool CollisionUtils::IsOverlapping(const Recti& r1, const Recti& r2)
{
	return	!
			((r1.x + r1.width) < r2.x ||
			(r2.x + r2.width) < r1.x ||
			r1.y > (r2.y + r2.height) ||
			r2.y > (r1.y + r1.height));
}