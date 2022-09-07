#include "Car.h"
#include "DxLib.h"

using namespace std;
Car::Car()
{
	const unsigned int color = GetColor(1.0f, 1.0f, 1.0f);
	colObject_ = make_unique<Capsule>(Vector3(), Vector3(), 1.0f, color);
}

Car::~Car()
{
}

void Car::Init(Vector3 startPos, Vector3 angle, float length, float radius)
{
	frontPos_ = startPos;
	angle_ = angle;
	carLength_ = length;

	CapsuleMove();
	colObject_->radius = radius;
}

void Car::Update()
{
	Vector3 ZVec(0.0f, 0.0f, 1.0f);
	frontPos_ += ZVec * 0.1f;

	CapsuleMove();
}

void Car::Finalize()
{
}

void Car::Draw()
{
	colObject_->draw();
}

void Car::CapsuleMove()
{
	colObject_->startPosition = frontPos_;
	colObject_->endPosition = (frontPos_ + (-angle_ * carLength_));

	centerPos_ = (frontPos_ + (-angle_ * (carLength_ / 2)));
}
