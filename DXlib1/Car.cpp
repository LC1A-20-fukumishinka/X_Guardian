#include "Car.h"
#include "DxLib.h"
#include "Matrix4.h"
#include <algorithm>
#include "EaseClass.h"
using namespace std;

const float Car::sTurnStartPos = 4.0f;
const float Car::sStopPos = 50.0f + sTurnStartPos;
const float Car::sStopLength = 3.0f;

const float Car::sPassWidth = sStopPos;
const float Car::sEraseWidth = 200.0f;
const float Car::sEraseDepth = 600.0f;
float Car::sGameSpeed = 1.0f;

MoveType Car::sInputSignal = MoveType::ALLOK;

const int Car::sMaxEnemyStopTimer = 60;
const int Car::sMaxDerayTimer = 35;

const float Car::sCarDistanceLimit = 10.0f;
vector<int> Car::sNormalCarModelHandles;
vector<int> Car::sTrackCarModelHandles;
vector<int> Car::sArrowModelHandle;

float Car::pressAnimationRate = 0.0f;

SoundManager *Car::sSounds = nullptr;
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
	model_ = ModelType::NORMAL;
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
	model_ = desc.model;
	enemyStopTimer_ = sMaxEnemyStopTimer;
	//カプセルの初期化処理
	colObject_->startPosition = frontPos_;
	colObject_->endPosition = (frontPos_ + (-angle_ * carLength_));
	centerPos_ = (frontPos_ + (-angle_ * (carLength_ * 0.55f)));
	isCrossIn_ = false;
	isCounted_ = false;
	clearAnimationRate_ = 0.0f;

	color_ = rand() % static_cast<int>(sNormalCarModelHandles.size());
	isCrossSound_ = false;


	isNowStop_ = false;
	isOldStop_ = false;
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

	isOldStop_ = isNowStop_;
	//止まるかどうか
	bool isStop = JudgmentToStop(isCrossIn);

	isNowStop_ = isStop;
	float inputSpeed = speed_;

	inputSpeed *= sGameSpeed;
	if (isPlayer_ && !isCrossIn && sInputSignal == MoveType::STOP )
	{
		inputSpeed /= 2.0f;
	}

	if (isStop)
	{
		if (isPlayer_ && isNowStop_ && !isOldStop_)
		{
			sSounds->EngineStop();
			sSounds->Brake(1);
		}
	}
	else
	{
		//前進処理
		frontPos_ += angle_ * inputSpeed;
	}


	float underFrame = -200.0f;
	bool isEnemySound = ((frontPos_.z <= underFrame + 70) && !isPlayer_ && !isCrossSound_);
	bool isPlayerSound = ((frontPos_.z >= underFrame) && isPlayer_ && !isCrossSound_);

	if (isPlayerSound || isEnemySound)
	{
		isCrossSound_ = true;
		sSounds->Engine(2);
	}
	bool isCrossTheIntersection = (isCrossIn_ && (abs(frontPos_.x) >= sEraseWidth || abs(frontPos_.z) >= sEraseDepth));
	if (isCrossTheIntersection)
	{
		Dead();
	}
	CapsuleMove();

	if (isCounted_)
	{
		clearAnimationRate_ += 0.05f;

		clearAnimationRate_ = std::clamp(clearAnimationRate_, 0.0f, 1.0f);
	}
}

void Car::Finalize()
{
}

void Car::Draw()
{

	int drawModelHandle = -1;
	int arrowModelHandle = -1;
	if (model_ == ModelType::NORMAL)
	{
		drawModelHandle = sNormalCarModelHandles[color_];
		arrowModelHandle = sArrowModelHandle[0];
	}
	else
	{
		drawModelHandle = sTrackCarModelHandles[color_];
		arrowModelHandle = sArrowModelHandle[1];
	}

	Matrix4 worldMat;
	Matrix4 arrowMat;
	float carScale = 0.04f;
	float pressEaseRate = Easing::easeOutCubic(pressAnimationRate);
	float pressScale = 0.01f;

	pressScale *= (1 - pressEaseRate);
	worldMat = scale(Vector3(carScale, carScale - pressScale, carScale));
	arrowMat = worldMat;


	if (model_ == ModelType::NORMAL)
	{
		arrowMat *= scale(Vector3(0.5f, 0.5f, 0.5f));
	}
	else
	{
		arrowMat *= scale(Vector3(0.5f, 0.5f, 0.5f));
	}
	Vector3 carAngle = (colObject_->endPosition - frontPos_);


	//通過時の回転
	if (isCounted_)
	{
		float easeRate = Easing::easeOutQuad(clearAnimationRate_);
		worldMat *= rotationZ(6.24f * easeRate);
	}

	//姿勢生成
	worldMat *= Posture(carAngle, Vector3(0.0f, 1.0f, 0.0f));
	worldMat *= rotationY(3.14f);


	if (model_ == ModelType::NORMAL)
	{
		arrowMat *= rotationY(-3.14f / 2.0f);
	}
	arrowMat *= rotationX(-0.5f);
	worldMat *= translate(centerPos_);
	arrowMat *= translate(centerPos_);

	arrowMat *= translate(Vector3(0.0f, 15.0f, 0.0f));
	if (model_ == ModelType::NORMAL)
	{
		arrowMat *= translate(Vector3(0.0f, -1.0f, 0.0f));
	}
	else
	{
		arrowMat *= translate(Vector3(5.5f / 2.0f, 3.0f, 0.0f));
	}
	//通過時の跳躍
	if (isCounted_)
	{

		Vector3 jump = Vector3(0.0f, 50.0f, 0.0f);

		float carHeight = 10.0f;
		if (model_ == ModelType::TRACK)
		{
			carHeight = 5.0f;
		}
		else
		{
			carHeight = 2.0f;
		}
		Vector3 uVec = Vector3(0.0f, -carHeight, 0.0f);
		Matrix4 uVecMat = translate(uVec);
		float easeRate = Easing::easeOutQuad(clearAnimationRate_);

		uVec = transform(uVec, rotationZ(6.24f * -easeRate));
		uVec = transform(uVec, Posture(carAngle, Vector3(0.0f, 1.0f, 0.0f)));
		uVec += Vector3(0.0f, carHeight, 0.0f);

		float jumpEaseRate = Easing::easeInOutQuad(clearAnimationRate_);

		if (jumpEaseRate >= 0.5f)
		{
			jumpEaseRate -= 0.5f;

			jumpEaseRate = 0.5f - jumpEaseRate;
		}
		uVec += (jump * jumpEaseRate);
		worldMat *= translate(uVec);
	}

	//車両描画
	MV1SetMatrix(drawModelHandle, worldMat);
	MV1DrawModel(drawModelHandle);


	if(isPlayer_ && !isCounted_)
	{
		//矢印描画
		MV1SetMatrix(arrowModelHandle, arrowMat);
		MV1DrawModel(arrowModelHandle);
	}
	//colObject_->draw();
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

ModelType Car::GetModelType()
{
	return model_;
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
	sSounds->Combo();
}

int Car::GetCarColor()
{
	return color_;
}

bool Car::GetIsPass()
{
	bool isPass = false;
	if (isCrossIn_ && !isCounted_)
	{
		if (isPlayer_)
		{
			isPass = (abs(colObject_->endPosition.x) >= sPassWidth || abs(colObject_->endPosition.z) > sStopPos);
		}
		else
		{
			isPass = (abs(colObject_->endPosition.x) >= sPassWidth || abs(colObject_->endPosition.z) > sStopPos);
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

void Car::SetGameSpeed(float speed)
{
	sGameSpeed = speed;
}

void Car::LoadModel(int arrow, int turnArrow)
{
	sNormalCarModelHandles.resize(5);
	sNormalCarModelHandles[0] = MV1LoadModel("Resources/cars/light blue/car.mv1");
	sNormalCarModelHandles[1] = MV1LoadModel("Resources/cars/light green/car.mv1");
	sNormalCarModelHandles[2] = MV1LoadModel("Resources/cars/pink/car.mv1");
	sNormalCarModelHandles[3] = MV1LoadModel("Resources/cars/purple/car.mv1");
	sNormalCarModelHandles[4] = MV1LoadModel("Resources/cars/yellow/car.mv1");

	sTrackCarModelHandles.resize(5);
	sTrackCarModelHandles[0] = MV1LoadModel("Resources/tracks/light blue/track.mv1");
	sTrackCarModelHandles[1] = MV1LoadModel("Resources/tracks/light green/track.mv1");
	sTrackCarModelHandles[2] = MV1LoadModel("Resources/tracks/pink/track.mv1");
	sTrackCarModelHandles[3] = MV1LoadModel("Resources/tracks/purple/track.mv1");
	sTrackCarModelHandles[4] = MV1LoadModel("Resources/tracks/yellow/track.mv1");
	//sTrackCarModelHandles = MV1LoadModel("Resources/track/track.mv1");
	sArrowModelHandle.resize(2);
	sArrowModelHandle[0] = arrow;
	sArrowModelHandle[1] = turnArrow;
	//sArrowModelHandle = MV1LoadModel("Resources/caution/caution.mv1");
}

void Car::SetPressAnimationRate(float rate)
{
	pressAnimationRate = rate;
}

void Car::SetSoundManager(SoundManager *sounds)
{
	sSounds = sounds;
}
void Car::CapsuleMove()
{
	colObject_->startPosition = frontPos_;

	Vector3 carAngle = (colObject_->endPosition - frontPos_);

	carAngle.normalaize();
	colObject_->endPosition = (frontPos_ + (carAngle * carLength_));

	centerPos_ = (frontPos_ + (carAngle * (carLength_ * 0.55f)));
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
		float inputspeed = speed_ * sGameSpeed;
		//旋回処理
		angle_ = angle_ * rotationY(0.02f * (inputspeed / 0.3f));
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
		bool inAria = (frontPos_.z <= sStopPos && frontPos_.z >= (sStopPos - sStopLength));
		isStopPosIn = (!isCrossIn && inAria);
	}

	//停止指示中
	bool isStopSignal = false;

	if (isPlayer_)
	{
		//プレイヤーだったら
		isStopSignal = !(type_ == sInputSignal || sInputSignal == MoveType::ALLOK);
	}
	else if (isStopPosIn && (enemyStopTimer_ > 0) && type_ == MoveType::RIGHTTURN)
	{
		enemyStopTimer_ -= static_cast<int>(1.0f * sGameSpeed);
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
			derayTimer_ -= 1 * sGameSpeed;
			isTrafficJam = true;
		}
	}


	//止まるかどうか
	bool isStop = (isSignal || isTrafficJam);

	return isStop;
}
