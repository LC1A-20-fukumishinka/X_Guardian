#include "RoadSignManager.h"

RoadSignManager::RoadSignManager()
{
	roadSign.resize(27);
	isChangeType = false;
}

RoadSignManager::~RoadSignManager()
{
}

void RoadSignManager::Init()
{
}

void RoadSignManager::Update(MoveType inputSignal)
{
	//if (RoadSign::sOldType != RoadSign::sType) {
	//	isChangeType = true;
	//}
	signType_ = inputSignal;
	if (oldSignType_ !=signType_) {

		//for (int i = 0; i < roadSign.size(); i++) {
		//	//Vector3 generatePos = Vector3(GetRand(-50, 50), -40, GetRand(-150, 150));
		//	
		//	//Vector3 generatePos = Vector3(-40, -40, 0);
		//	//Vector3 generatePos = GetRoadSignPos(i);
		//	//generatePos.y = -40;
		//	//roadSign[i].Init(generatePos,signType_);
		//	roadSign[i].Init(Vector3(-27.5f, -40, -120), signType_);
		//}
		float y = -50.0f;
		roadSign[0].Init(Vector3(-27.5f, y, -120), signType_);
		roadSign[1].Init(Vector3(-27.5f, y, -82.5f), signType_);
		roadSign[2].Init(Vector3(-27.5f, y, -45), signType_);
		roadSign[3].Init(Vector3(27.5f, y, -120), signType_);
		roadSign[4].Init(Vector3(27.5f, y, -82.5f), signType_);
		roadSign[5].Init(Vector3(27.5f, y, -45), signType_);
		roadSign[6].Init(Vector3(65, y, -42.5f), signType_);
		roadSign[7].Init(Vector3(102.5f, y, -42.5f), signType_);
		roadSign[8].Init(Vector3(-65, y, -42.5f), signType_);
		roadSign[9].Init(Vector3(-102.5f, y, 42.5f), signType_);
		roadSign[10].Init(Vector3(-140, y, -42.5f), signType_);
		roadSign[11].Init(Vector3(-65, y, 42.5f), signType_);
		roadSign[12].Init(Vector3(-102.5f, y, -42.5f), signType_);
		roadSign[13].Init(Vector3(-140, y, 42.5f), signType_);
		roadSign[14].Init(Vector3(65, y, 42.5f), signType_);
		roadSign[15].Init(Vector3(102.5f, y, 42.5f), signType_);
		roadSign[16].Init(Vector3(140, y, 42.5f), signType_);
		roadSign[17].Init(Vector3(27.5f, y, 42.5f), signType_);
		roadSign[18].Init(Vector3(27.5f, y, 267.5f), signType_);
		roadSign[19].Init(Vector3(27.5f, y, 117.5f), signType_);
		roadSign[20].Init(Vector3(27.5f, y, 342.5f), signType_);
		roadSign[21].Init(Vector3(27.5f, y, 192.5f), signType_);
		roadSign[22].Init(Vector3(-27.5f, y, 42.5f), signType_);
		roadSign[23].Init(Vector3(-27.5f, y, 267.5f), signType_);
		roadSign[24].Init(Vector3(-27.5f, y, 117.5f), signType_);
		roadSign[25].Init(Vector3(-27.5f, y, 342.5f), signType_);
		roadSign[26].Init(Vector3(-27.5f, y, 192.5f), signType_);
	}
	oldSignType_ = signType_;
	for (int i = 0; i < roadSign.size(); i++) {
		roadSign[i].Update();
	}
}

void RoadSignManager::Finalize()
{
}

void RoadSignManager::Draw()
{
	for (int i = 0; i < roadSign.size(); i++) {
		roadSign[i].Draw();
	}
}

void RoadSignManager::ScaleAnimation(float easeRate)
{
	for (int i = 0; i < roadSign.size(); i++) {
		roadSign[i].ScaleAnimation(easeRate);
	}
}
