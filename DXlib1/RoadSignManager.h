#pragma once
#include "RoadSign.h"
#include <vector>
#include "Matrix4.h"

class RoadSignManager
{
public:
	std::vector<RoadSign> roadSign;

	RoadSignManager();
	~RoadSignManager();

	void Init();
	void Update(MoveType inputSignal);
	void Finalize();
	void Draw();

	bool isChangeType;
	MoveType signType_;
	MoveType oldSignType_;
	int num = 5;

	inline int GetRand(const int& min, const int& max) {
		return (rand() % (max - min + 1)) + min;
	}

	Vector3 GetRoadSignPos(int ObjNum)
	{
		Vector3 v = Vector3(0, 0, 0);
		//int num = 7;

		if (ObjNum < num)
		{
			v.x = GetRand(-45, -35);
			v.z = GetRand(-130, -35);
			return v;
		}
		else if (ObjNum > num&&ObjNum < num*2)
		{
			v.x = GetRand(35, 45);
			v.z = GetRand(-130, -35);
			return v;
		}
		else if (ObjNum > num*2&&ObjNum < num*3)
		{
			v.x = GetRand(-45, -35);
			v.z = GetRand(35, 300);
			return v;
		}
		else
		{
			v.x = GetRand(35, 45);
			v.z = GetRand(35, 300);
			return v;
		}

	}

	void ScaleAnimation(float easeRate);
};