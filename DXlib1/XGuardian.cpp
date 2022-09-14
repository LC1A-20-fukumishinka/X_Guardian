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
	var_ = 0;
	angle_ = 0.0f;
	spinTimer_ = 0;
	isSpin_ = false;
	xGuardianAnimScale_ = 0.05;
}

XGuardian::~XGuardian()
{
}

void XGuardian::Init()
{
	centerPos_ = Vector3(18, 30, -130);
}

void XGuardian::Update(MoveType inputSignal)
{
	inputSignal_ = inputSignal;
	MV1SetPosition(XGuardianModelHandle_, centerPos_);
	MV1SetPosition(XGuardiansStopModelHandle_, centerPos_);
	MV1SetPosition(XGuardiansStraightModelHandle_, centerPos_);
	MV1SetPosition(XGuardiansTurnRightModelHandle_, centerPos_);

	centerPos_.y = sin(var_ * (DX_PI_F / 180.0f)) * 2 + 30;
	var_ += 4.0f;

	Spin();
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

	float XScale = 0.1f;

	Matrix4 matRot = rotationY(0.314f);
	matRot *= rotationX(0.314f);
	worldMat = scale(Vector3(XScale, XScale - xGuardianAnimScale_, XScale));

	worldMat *= rotationY(15 / 180 * 3.14);
	worldMat *= matRot;


	worldMat *= translate(centerPos_);

	Matrix4 sWorldMat;

	sWorldMat = scale(Vector3(XScale, XScale - xGuardianAnimScale_, XScale));


	sWorldMat *= rotationY(15 / 180 * 3.14);

	sWorldMat *= rotationY(angle_);
	sWorldMat *= matRot;


	//angle_ += 2.0f;

	sWorldMat *= translate(centerPos_);


	MV1SetMatrix(XGuardianModelHandle_, worldMat);
	MV1SetMatrix(XGuardiansStopModelHandle_, sWorldMat);
	MV1SetMatrix(XGuardiansStraightModelHandle_, sWorldMat);
	MV1SetMatrix(XGuardiansTurnRightModelHandle_, sWorldMat);
	
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

void XGuardian::Spin()
{
	if (oldInputSignal_ != inputSignal_&&!isSpin_) {
		isSpin_ = true;
	}
	oldInputSignal_ = inputSignal_;
	if (isSpin_) {
		spinTimer_++;
		if (spinTimer_ <= MAX_SPINTIMER) {
			angle_ += 2.0f;
		}
		if (spinTimer_ > MAX_SPINTIMER) {
			isSpin_ = false;
			spinTimer_ = 0;
			angle_ = 0.0f;
		}
	}
}

void XGuardian::ScaleAnimation(float easeRate)
{
	xGuardianAnimScale_ = 0.025f;
	xGuardianAnimScale_ *= (1.0f - easeRate);
}
