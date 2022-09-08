#include "Car.h"
#include "DxLib.h"
#include "Matrix4.h"
using namespace std;
Car::Car()
{
	const unsigned int color = GetColor(255, 255, 255);
	colObject_ = make_unique<Capsule>(Vector3(), Vector3(), 1.0f, color);
}

Car::~Car()
{
}

void Car::Init(Vector3 startPos, Vector3 angle, float length, float radius, float speed, bool isPlayer, MoveType type)
{
	frontPos_ = startPos;
	angle_ = angle;
	carLength_ = length;

	colObject_->radius = radius;
	speed_ = speed;
	isAlive_ = true;
	isPlayer_ = isPlayer;
	type_ = type;


	//カプセルの初期化処理
	colObject_->startPosition = frontPos_;
	colObject_->endPosition = (frontPos_ + (-angle_ * carLength_));
	centerPos_ = (frontPos_ + (-angle_ * (carLength_ / 2)));
}

void Car::Init(CarInitializeDesc desc)
{
	frontPos_ = desc.startPos;
	angle_ = desc.angle;
	carLength_ = desc.length;

	colObject_->radius = desc.radius;
	speed_ = desc.speed;
	isAlive_ = true;
	isPlayer_ = desc.isPlayer;
	type_ = desc.type;

	//カプセルの初期化処理
	colObject_->startPosition = frontPos_;
	colObject_->endPosition = (frontPos_ + (-angle_ * carLength_));
	centerPos_ = (frontPos_ + (-angle_ * (carLength_ / 2)));
}

void Car::Update()
{

	//交差点に進入したか
	bool isCrossIn;

	bool isStopPosIn;
	//交差点で曲がり始める位置
	const float turnStartPos = 5.0f;

	//停止する位置
	const float stopPos = 10.0f + turnStartPos;

	//交差点に進入しているかどうかの判断

	if (isPlayer_)
	{
		isCrossIn = ( frontPos_.z >= -turnStartPos);
		isStopPosIn = (!isCrossIn && frontPos_.z >= -stopPos);
	}
	else
	{
		isCrossIn = ( frontPos_.z <= turnStartPos);
		isStopPosIn = (!isCrossIn && frontPos_.z <= stopPos);
	}
	if (isCrossIn)
	{
		switch (type_)
		{
		case MoveType::STRAIGHT:
			StraightMove();
			break;
		case MoveType::RIGHTTURN:
			RightTurnMove();
			break;
		default:
			StraightMove();
			break;
		}
	}


	//信号が赤だった    交差点に未侵入 & 停止位置 &
	bool isSignal = (isStopPosIn && isStopSignal_);

	//前が詰まった
	bool isTrafficJam = false;

	//前の車両が存在していたら
	if (!frontCar_.expired() && frontCar_.lock()->GetIsAlive())
	{
		//二台の車両の距離
		Vector3 distance = (frontCar_.lock()->GetTailPos() - GetFrontPos());

		float carDistanceLimit = 5.0f;

		//距離が近すぎるか確認
		isTrafficJam = (distance.length() <= carDistanceLimit);
	}


	//止まるかどうか
	bool isStop = (isSignal || isTrafficJam);

	if (isStop)
	{

	}
	else
	{
		//前進処理
		frontPos_ += angle_ * speed_;
	}

	CapsuleMove();
}

void Car::Finalize()
{
}

void Car::Draw()
{
	colObject_->draw();
}

bool Car::GetIsAlive()
{
	return isAlive_;
}

Vector3 Car::GetTailPos()
{
	Vector3 tailPos = colObject_->endPosition;

	tailPos += (-angle_ * colObject_->radius);
	return tailPos;
}

Vector3 Car::GetFrontPos()
{
	Vector3 frontPos = colObject_->startPosition;

	frontPos += (angle_ * colObject_->radius);
	return frontPos;
}

MoveType Car::GetMoveType()
{
	return type_;
}

void Car::SetFrontCar(std::weak_ptr<Car> frontCar)
{
	frontCar_ = frontCar;
}

void Car::SetSignal(bool isStopSignal)
{
	isStopSignal_ = isStopSignal;
}

void Car::CapsuleMove()
{
	colObject_->startPosition = frontPos_;

	Vector3 carAngle = (colObject_->endPosition - frontPos_);

	carAngle.normalaize();
	colObject_->endPosition = (frontPos_ + (carAngle * carLength_));

	centerPos_ = (frontPos_ + (carAngle * (carLength_ / 2)));
}

void Car::StraightMove()
{
}

void Car::RightTurnMove()
{
	bool isTurnOver;

	if (isPlayer_)
	{
		isTurnOver = (angle_.z <= 0.0f);
	}
	else
	{
		isTurnOver = (angle_.z >= 0.0f);
	}
	if (isTurnOver)
	{
		angle_.z = 0.0f;
	}
	else
	{
		//旋回処理
		angle_ = angle_ * rotationY(0.02f * (speed_ / 0.3f));
	}
}
