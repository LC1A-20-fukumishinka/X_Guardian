#include "Car.h"
#include "DxLib.h"
#include "Matrix4.h"
using namespace std;

const float Car::sTurnStartPos = 5.0f;
const float Car::sStopPos = 20.0f + sTurnStartPos;
const float Car::sStopLength = 10.0f;

const float Car::sPassWidth = sStopPos;
const float Car::sEraseWidth = 100.0f;
const float Car::sEraseDepth = 600.0f;


MoveType Car::sInputSignal = MoveType::STRAIGHT;

const int Car::sMaxEnemyStopTimer = 120;
const int Car::sMaxDerayTimer = 30;

const float Car::sCarDistanceLimit = 10.0f;
int Car::sNormalCarModelHandle = -1;
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

	enemyStopTimer_ = 240;
	//カプセルの初期化処理
	colObject_->startPosition = frontPos_;
	colObject_->endPosition = (frontPos_ + (-angle_ * carLength_));
	centerPos_ = (frontPos_ + (-angle_ * (carLength_ / 2)));
	isCrossIn_ = false;
	isCounted_ = false;
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

	enemyStopTimer_ = sMaxEnemyStopTimer;
	//カプセルの初期化処理
	colObject_->startPosition = frontPos_;
	colObject_->endPosition = (frontPos_ + (-angle_ * carLength_));
	centerPos_ = (frontPos_ + (-angle_ * (carLength_ / 2)));
	isCrossIn_ = false;
	isCounted_ = false;
}

void Car::Update()
{
	//交差点に進入したか
	bool isCrossIn;
	//交差点に進入しているかどうかの判断

	if (isPlayer_)
	{
		isCrossIn = (frontPos_.z >= -sTurnStartPos);
	}
	else
	{
		isCrossIn = (frontPos_.z <= sTurnStartPos);
	}
	if (isCrossIn)
	{
		isCrossIn_ = true;
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



	//止まるかどうか
	bool isStop = JudgmentToStop(isCrossIn);

	float inputSpeed = speed_;

	if (isPlayer_ && !isCrossIn && sInputSignal == MoveType::STOP)
	{
		inputSpeed /= 2.0f;
	}

	if (isStop)
	{

	}
	else
	{
		//前進処理
		frontPos_ += angle_ * inputSpeed;
	}

	bool isCrossTheIntersection = (isCrossIn_ && (abs(frontPos_.x) >= sEraseWidth || abs(frontPos_.z) >= sEraseDepth));
	if (isCrossTheIntersection)
	{
		Dead();
	}
	CapsuleMove();
}

void Car::Finalize()
{
}

void Car::Draw()
{
	Matrix4 worldMat;
	worldMat = scale(Vector3(0.04f, 0.04f, 0.04f));

	Vector3 carAngle = (colObject_->endPosition - frontPos_);
	worldMat *= Posture(carAngle, Vector3(0.0f, 1.0f, 0.0f));
	worldMat *= rotationY(3.14);
	worldMat *= translate(centerPos_);
	MV1SetMatrix(sNormalCarModelHandle, worldMat);
	MV1DrawModel(sNormalCarModelHandle);
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

Capsule *Car::GetCapsule()
{
	return colObject_.get();
}

void Car::SetFrontCar(std::weak_ptr<Car> frontCar)
{
	frontCar_ = frontCar;
}

void Car::Dead()
{
	isAlive_ = false;
}

void Car::Count()
{
	isCounted_ = true;
}

bool Car::GetIsPass()
{
	bool isPass = false;
	if (isCrossIn_ && !isCounted_)
	{
		if (isPlayer_)
		{
			isPass = (abs(frontPos_.x) >= sPassWidth || abs(frontPos_.z) > sStopPos);
		}
		else
		{
			isPass = (abs(frontPos_.x) >= sPassWidth || abs(frontPos_.z) > sStopPos);
		}
	}
	return isPass;
}

bool Car::GetIsSignalStop()
{
	bool isStopSignal = false;


	if (isPlayer_)
	{
		bool isStopPosIn;

		bool inAria = (frontPos_.z >= -sStopPos && frontPos_.z < (-sStopPos + sStopLength));
		isStopPosIn = (!isCrossIn_ && inAria);

		//停止位置 && 自分の進行方向と指示が一致していなかったら
		if (isStopPosIn && (type_ != sInputSignal))
		{
			isStopSignal = true;
		}
	}

	return isStopSignal;
}

void Car::SetSignal(MoveType isStopSignal)
{
	sInputSignal = isStopSignal;
}

void Car::LoadModel()
{
	sNormalCarModelHandle = MV1LoadModel("car/car.mv1");
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


bool Car::JudgmentToStop(bool isCrossIn)
{
	bool isStopPosIn;

	//交差点に進入しているかどうかの判断

	if (isPlayer_)
	{
		bool inAria = (frontPos_.z >= -sStopPos && frontPos_.z < (-sStopPos + sStopLength));
		isStopPosIn = (!isCrossIn && inAria);
	}
	else
	{
		bool inAria = (frontPos_.z <= sStopPos && frontPos_.z > (sStopPos - sStopLength));
		isStopPosIn = (!isCrossIn && inAria);
	}

	//停止指示中
	bool isStopSignal = false;

	if (isPlayer_)
	{
		//プレイヤーだったら
		isStopSignal = !(type_ == sInputSignal);
	}
	else if (isStopPosIn && (enemyStopTimer_ > 0) && type_ == MoveType::RIGHTTURN)
	{
		enemyStopTimer_--;
		isStopSignal = true;
	}


	//信号が赤だった    停止位置 & 停止指示中だった
	bool isSignal = (isStopPosIn && isStopSignal);

	//前が詰まった
	bool isTrafficJam = false;

	//前の車両が存在していたら
	if (!frontCar_.expired() && frontCar_.lock()->GetIsAlive())
	{
		//二台の車両の距離
		Vector3 distance = (frontCar_.lock()->GetTailPos() - GetFrontPos());


		//距離が近すぎるか確認
		if ((distance.length() <= sCarDistanceLimit))
		{
			derayTimer_ = sMaxDerayTimer;
		}

		if (derayTimer_ > 0)
		{
			derayTimer_--;
			isTrafficJam = true;
		}
	}


	//止まるかどうか
	bool isStop = (isSignal || isTrafficJam);

	return isStop;
}
