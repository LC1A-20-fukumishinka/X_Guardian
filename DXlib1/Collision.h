#pragma once
#include "Primitive.h"
#include "Vector3.h"

//è’ìÀîªíËÉNÉâÉX
class Collision
{
public:

	static bool testSphereSphere(const Sphere &sphere1, const Sphere &sphere2);

	static bool testSphereBox(const Sphere &sphere, const Box &box);

	static bool testSphereCapsule(const Sphere &sphere, const Capsule &capsule);

	static bool testCapsuleCapsule(const Capsule &capsule1, const Capsule & capsule2);

	static float sqDistanceSegmentSegment(const Vector3 &p1, const Vector3 &q1, const Vector3 &p2, const Vector3 &q2);


	static float clamp(float x, float low, float high)
	{
		x = (x <low) ? low : x;
		x = (x > high) ? high : x;
		return x;
	}
};
