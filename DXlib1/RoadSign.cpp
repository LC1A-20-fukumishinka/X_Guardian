#include "RoadSign.h"

int RoadSign::sStopModelHandle = -1;
int RoadSign::sStraightModelHandle = -1;
int RoadSign::sTurnRightModelHandle = -1;

void RoadSign::LoadModel()
{
	sStopModelHandle = MV1LoadModel("Resources/road_sign/sign_stop.mv1");
	sStraightModelHandle = MV1LoadModel("Resources/road_sign/sign_straight.mv1");
	sTurnRightModelHandle = MV1LoadModel("Resources/road_sign/sign_turnright.mv1");
}

RoadSign::RoadSign()
{
	centerPos_ = Vector3(0, 0, 0);
	sType = MoveType::STOP;
}

RoadSign::~RoadSign()
{
}

void RoadSign::Init(Vector3 pos,MoveType type)
{
	centerPos_ = pos;
	sType = type;
}

void RoadSign::Update()
{
	if (centerPos_.y <= 0) { centerPos_.y += 8; }
}

void RoadSign::Draw()
{

	int drawModelHandle = -1;

	switch (sType)
	{
	case MoveType::STRAIGHT:
		drawModelHandle = sStraightModelHandle;
		break;
	case MoveType::RIGHTTURN:
		drawModelHandle = sTurnRightModelHandle;
		break;
	case MoveType::STOP:
		drawModelHandle = sStopModelHandle;
		break;
	default:
		break;
	}

	if (sType == MoveType::STOP)
	{
		
	}
	if(sType==MoveType::STRAIGHT)
	{
	}
	if(sType == MoveType::RIGHTTURN)
	{
	}

	Matrix4 worldMat;

	float carScale = 0.08f;
	worldMat = scale(Vector3(carScale, carScale - signAnimScale_, carScale));

	worldMat *= rotationY(3.14);


	worldMat *= translate(centerPos_);

	MV1SetMatrix(drawModelHandle, worldMat);
	MV1DrawModel(drawModelHandle);
}

void RoadSign::ScaleAnimation(float easeRate)
{
	signAnimScale_ = 0.025f;
	signAnimScale_ *= (1.0f - easeRate);
}

