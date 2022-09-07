#pragma once
#include "Vector3.h"
#include "Primitive.h"
#include <memory>
class Car
{
public:
	Car();
	~Car();
	void Init(Vector3 startPos, Vector3 angle, float length, float radius);
	void Update();
	void Finalize();
	void Draw();
private:

	void CapsuleMove();
	//カプセル状の当たり判定使わないかも
std::unique_ptr<Capsule> colObject_;
//車両の先頭部分の座標
Vector3 frontPos_;
//車両の方向
Vector3 angle_;

Vector3 centerPos_;
float carLength_;


};

