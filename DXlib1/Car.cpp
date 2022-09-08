#include "Car.h"
#include "DxLib.h"
#include "Matrix4.h"
using namespace std;

const float Car::sTurnStartPos = 5.0f;
const float Car::sStopPos = 15.0f + sTurnStartPos;

MoveType Car::sInputSignal = MoveType::STRAIGHT;

const int Car::sMaxEnemyTimer = 120;
const 	int sMaxDerayTimer_ = 30;

const float Car::sCarDistanceLimit = 5.0f;

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
	//�J�v�Z���̏���������
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

	enemyStopTimer_ = sMaxEnemyTimer;
	//�J�v�Z���̏���������
	colObject_->startPosition = frontPos_;
	colObject_->endPosition = (frontPos_ + (-angle_ * carLength_));
	centerPos_ = (frontPos_ + (-angle_ * (carLength_ / 2)));
}

void Car::Update()
{

	//�����_�ɐi��������
	bool isCrossIn;
	//�����_�ɐi�����Ă��邩�ǂ����̔��f

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



	//�~�܂邩�ǂ���
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
		//�O�i����
		frontPos_ += angle_ * inputSpeed;
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

void Car::SetSignal(MoveType isStopSignal)
{
	sInputSignal = isStopSignal;
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
		//���񏈗�
		angle_ = angle_ * rotationY(0.02f * (speed_ / 0.3f));
	}
}


bool Car::JudgmentToStop(bool isCrossIn)
{
	bool isStopPosIn;

	//�����_�ɐi�����Ă��邩�ǂ����̔��f

	if (isPlayer_)
	{
		isStopPosIn = (!isCrossIn && frontPos_.z >= -sStopPos);
	}
	else
	{
		isStopPosIn = (!isCrossIn && frontPos_.z <= sStopPos);
	}

	//��~�w����
	bool isStopSignal = false;

	if (isPlayer_)
	{
		//�v���C���[��������
		isStopSignal = !(type_ == sInputSignal);
	}
	else if (isStopPosIn && (enemyStopTimer_ > 0) && type_ == MoveType::RIGHTTURN)
	{
		enemyStopTimer_--;
		isStopSignal = true;
	}


	//�M�����Ԃ�����    ��~�ʒu & ��~�w����������
	bool isSignal = (isStopPosIn && isStopSignal);

	//�O���l�܂���
	bool isTrafficJam = false;

	//�O�̎ԗ������݂��Ă�����
	if (!frontCar_.expired() && frontCar_.lock()->GetIsAlive())
	{
		//���̎ԗ��̋���
		Vector3 distance = (frontCar_.lock()->GetTailPos() - GetFrontPos());


		//�������߂����邩�m�F
		if ((distance.length() <= sCarDistanceLimit))
		{
			derayTimer_ = sMaxDerayTimer_;
		}

		if (derayTimer_ > 0)
		{
			derayTimer_--;
			isTrafficJam = true;
		}
	}


	//�~�܂邩�ǂ���
	bool isStop = (isSignal || isTrafficJam);

	return isStop;
}
