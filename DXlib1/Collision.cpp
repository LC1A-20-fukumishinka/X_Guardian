#include "Collision.h"

bool Collision::testSphereSphere(const Sphere &sphere1, const Sphere &sphere2)
{
	Vector3 vec = sphere2.position - sphere1.position;

	float sqLength = vec.length();

	float r = sphere1.radius + sphere2.radius;

	return sqLength <= r;
}

bool Collision::testSphereBox(const Sphere &sphere, const Box &box)
{
	float sqDistance = 0.0f;
	float pos;

	pos = sphere.position.x;
	if (pos < box.minPosition.x)
	{
		sqDistance += pow(box.minPosition.x - pos, 2.0);
	}
	else if (pos > box.maxPosition.x)
	{
		sqDistance += pow(pos - box.maxPosition.x, 2.0);
	}

	pos = sphere.position.y;
	if (pos < box.minPosition.y)
	{
		sqDistance += pow(box.minPosition.y - pos, 2.0);
	}
	else if (pos > box.maxPosition.y)
	{
		sqDistance += pow(pos - box.maxPosition.y, 2.0);
	}

	pos = sphere.position.z;
	if (pos < box.minPosition.z)
	{
		sqDistance += pow(box.minPosition.z - pos, 2.0);
	}
	else if (pos > box.maxPosition.z)
	{
		sqDistance += pow(pos - box.maxPosition.z, 2.0);
	}

	return sqDistance <= sphere.radius * sphere.radius;
}

bool Collision::testSphereCapsule(const Sphere &sphere, const Capsule &capsule)
{
	//1.カプセル内の線分のsたーとの位置から円土井珍\へのベクトルを作る
	Vector3 vStartToEnd = capsule.endPosition - capsule.startPosition;

	//２．１。のべくとるを体にベクトルか(normalize)視。n二用意する
	Vector3 n = vStartToEnd;
	n.normalaize();

	float t = n.dot(sphere.position - capsule.startPosition);

	Vector3 vPsPn = n * t;

	Vector3 posPn = capsule.startPosition + vPsPn;

	float lengthRate = t / vStartToEnd.length();

	float distance;
	if (lengthRate < 0.0f)
	{
		distance = Vector3(sphere.position - capsule.startPosition).length() - capsule.radius;
	}
	else if (lengthRate <= 1.0f)
	{
		distance = Vector3(sphere.position - posPn).length() - capsule.radius;
	}
	else
	{
		distance = Vector3(sphere.position - capsule.endPosition).length() - capsule.radius;
	}
	return (distance <= sphere.radius);
}

bool Collision::testCapsuleCapsule(const Capsule &capsule1, const Capsule &capsule2)
{
	float sqDistance =
		sqDistanceSegmentSegment(capsule1.startPosition, capsule1.endPosition, capsule2.startPosition, capsule2.endPosition);

	float r = capsule1.radius + capsule2.radius;
	return ((r * r) >= sqDistance);
}

float Collision::sqDistanceSegmentSegment(const Vector3 &p1, const Vector3 &q1, const Vector3 &p2, const Vector3 &q2)
{
	Vector3 d1 = q1 - p1;
	Vector3 d2 = q2 - p2;

	Vector3 r = p1 - p2;

	float a = d1.dot(d1);
	float b = d1.dot(d2);
	float e = d2.dot(d2);

	float c = d1.dot(r);
	float f = d2.dot(r);

	float s = 0.0f;
	float t = 0.0f;

	float denominator = (a * e) - (b * b);

	if (denominator != 0.0f)
	{
		s = ((b * f) - (c * e)) / denominator;
		s = clamp(s, 0.0f, 1.0f);
	}
	else
	{
		s = 0.0f;
	}

	t = (f + ( b * s )) / e;

	if (t < 0.0f)
	{
		t = 0.0f;
		s = clamp(-c / a, 0.0f, 1.0f);
	}
	else if (t > 1.0)
	{
		s = clamp((b - c) / a, 0.0f, 1.0f);
		t = 1.0f;
	}
	Vector3 c1 = p1 + (s * d1);
	Vector3 c2 = p2 + (t * d2);

	c2 -= c1;

	float distance = c2.dot(c2);
	return distance;
}
