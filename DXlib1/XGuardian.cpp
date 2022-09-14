#include"XGuardian.h"

//MoveType XGuardian::inputSignal_ = MoveType::STRAIGHT;
//int XGuardian::XGuardianModelHandle_ = -1;
//int XGuardian::XGuardiansShieldModelHandle_ = -1;

void XGuardian::LoadModel()
{
	XGuardianModelHandle_ = MV1LoadModel("Resources/XGuardian/XGuardian.mv1");
	XGuardiansStopModelHandle_ = MV1LoadModel("Resources/XGuardian/stop/XGuardiansShield.mv1");
	XGuardiansStraightModelHandle_ = MV1LoadModel("Resources/XGuardian/straight/XGuardiansShield.mv1");
	XGuardiansTurnRightModelHandle_ = MV1LoadModel("Resources/XGuardian/TurnRight/XGuardiansShield.mv1");
}

XGuardian::XGuardian()
{
	inputSignal_ = MoveType::STRAIGHT;
	XGuardianModelHandle_ = -1;
	XGuardiansStopModelHandle_ = -1;
	XGuardiansStraightModelHandle_ = -1;
	XGuardiansTurnRightModelHandle_ = -1;
	var = 0;
}

XGuardian::~XGuardian()
{
}

void XGuardian::Init()
{
	centerPos_ = Vector3(45, 38, -120);
}

void XGuardian::Update(MoveType inputSignal)
{
	inputSignal_ = inputSignal;
	MV1SetPosition(XGuardianModelHandle_, centerPos_);
	MV1SetPosition(XGuardiansStopModelHandle_, centerPos_);
	MV1SetPosition(XGuardiansStraightModelHandle_, centerPos_);
	MV1SetPosition(XGuardiansTurnRightModelHandle_, centerPos_);

	centerPos_.y = sin(var * (DX_PI_F / 180.0f)) * 2 + 38;
	var += 4.0f;
}

void XGuardian::Finalize()
{
	//ƒ‚ƒfƒ‹‚ðÁ‚·
	MV1DeleteModel(XGuardianModelHandle_);
	MV1DeleteModel(XGuardiansStopModelHandle_);
	MV1DeleteModel(XGuardiansStraightModelHandle_);
	MV1DeleteModel(XGuardiansTurnRightModelHandle_);
}

void XGuardian::Draw()
{
	Matrix4 worldMat;

	float XScale = 0.1f;/*
	float pressEaseRate = Easing::easeOutCubic(pressAnimationRate);
	float pressScale = 0.01f;*/

	//pressScale *= (1 - pressEaseRate);
	worldMat = scale(Vector3(XScale, XScale, XScale));

	//Vector3 carAngle = (colObject_->endPosition - frontPos_);

	//if (isCounted_)
	//{
	//	float easeRate = Easing::easeOutQuad(clearAnimationRate_);
	//	worldMat *= rotationZ(6.24f * easeRate);
	//}
	//worldMat *= Posture(carAngle, Vector3(0.0f, 1.0f, 0.0f));
	worldMat *= rotationY(3.14f/2.5f);


	worldMat *= translate(centerPos_);

	//if (isCounted_)
	//{

	//	Vector3 jump = Vector3(0.0f, 50.0f, 0.0f);

	//	float carHeight = 10.0f;
	//	if (model_ == ModelType::TRACK)
	//	{
	//		carHeight = 5.0f;
	//	}
	//	else
	//	{
	//		carHeight = 2.0f;
	//	}
	//	Vector3 uVec = Vector3(0.0f, -carHeight, 0.0f);
	//	Matrix4 uVecMat = translate(uVec);

	//	worldMat *= translate(uVec);
	//}


	MV1SetMatrix(XGuardianModelHandle_, worldMat);
	MV1SetMatrix(XGuardiansStopModelHandle_, worldMat);
	MV1SetMatrix(XGuardiansStraightModelHandle_, worldMat);
	MV1SetMatrix(XGuardiansTurnRightModelHandle_, worldMat);
	
	//colObject_->draw();
	MV1DrawModel(XGuardianModelHandle_);
	switch (inputSignal_)
	{
	case MoveType::STOP:
		MV1DrawModel(XGuardiansStopModelHandle_);
		break;
	case MoveType::RIGHTTURN:
		MV1DrawModel(XGuardiansTurnRightModelHandle_);
		break;
	case MoveType::STRAIGHT:
		MV1DrawModel(XGuardiansStraightModelHandle_);
		break;
	default:
		break;
	}
}
