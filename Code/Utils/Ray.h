#ifndef RAY_H
#define RAY_H

#include <CryMath/Cry_Math.h>

struct SRay {
	Vec3 origin_{ ZERO };
	Vec3 direction_{ ZERO };
};

#endif // !RAY_H