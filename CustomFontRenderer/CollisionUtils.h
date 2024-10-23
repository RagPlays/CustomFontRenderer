#ifndef COLLISIONUTILS_H
#define COLLISIONUTILS_H

#include "Structs.h"

class CollisionUtils final
{
public:

	// All static collision functions //

	static bool IsOverlapping(const Recti& r1, const Recti& r2);

};

#endif // !COLLISIONUTILS_H
