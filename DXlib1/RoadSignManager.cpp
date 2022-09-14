#include "RoadSignManager.h"

RoadSignManager::RoadSignManager()
{
	roadSign.resize(num * 4);
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

		for (int i = 0; i < roadSign.size(); i++) {
			//Vector3 generatePos = Vector3(GetRand(-50, 50), -40, GetRand(-150, 150));
			
			//Vector3 generatePos = Vector3(-40, -40, 0);
			Vector3 generatePos = GetRoadSignPos(i);
			generatePos.y = -40;
			roadSign[i].Init(generatePos,signType_);
		}
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
