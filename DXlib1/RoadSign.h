#pragma once
#include "CarManager.h"

class RoadSign
{
public:
	static int sStopModelHandle;
	static int sStraightModelHandle;
	static int sTurnRightModelHandle;
	static void LoadModel();
	MoveType sType;;

	float signAnimScale_;
	bool isAlive = false;

	Vector3 centerPos_;
	RoadSign();
	~RoadSign();
	void Init(Vector3 pos,MoveType type);
	void Update();
	void Draw();

	void ScaleAnimation(float easeRate);
};