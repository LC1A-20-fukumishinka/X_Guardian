#pragma once
#include "Vector3.h"
#include "Primitive.h"
#include <memory>
enum class MoveType
{
	STRAIGHT,
	RIGHTTURN,
	STOP,
};

struct CarInitializeDesc
{
	Vector3 startPos = Vector3();
	Vector3 angle = Vector3(0, 0, 1.0f);
	float length = 10.0f;
	float radius = 3.0f;
	float speed = 0.05f;
	bool isPlayer = true;
	MoveType type = MoveType::STRAIGHT;
};
class Car
{
public:

	Car();
	~Car();
	void Init(Vector3 startPos, Vector3 angle, float length, float radius, float speed, bool isPlayer, MoveType type);
	void Init(CarInitializeDesc desc);
	void Update();
	void Finalize();
	void Draw();

	bool GetIsAlive();

	Vector3 GetTailPos();

	Vector3 GetFrontPos();

	MoveType GetMoveType();

	Capsule *GetCapsule();
	void SetFrontCar(std::weak_ptr<Car> frontCar);

	void Dead();
private:
	static const float sTurnStartPos;
	static const float sStopPos;
	static MoveType sInputSignal;
	static const int sMaxEnemyTimer;
	static const float sCarDistanceLimit;
	static const int sMaxDerayTimer_ = 30;

public:
	static void SetSignal(MoveType isStopSignal);
private:

	void CapsuleMove();

	void StraightMove();

	void RightTurnMove();

	bool JudgmentToStop(bool isCrossIn);


	int enemyStopTimer_ = 60;
	//カプセル状の当たり判定使わないかも
	std::unique_ptr<Capsule> colObject_;
	//車両の先頭部分の座標
	Vector3 frontPos_;
	//車両の方向
	Vector3 angle_;

	Vector3 centerPos_;
	float carLength_;

	float speed_;

	bool isAlive_ = false;
	bool isPlayer_ = true;

	int derayTimer_ = 0;
	MoveType type_ = MoveType::STRAIGHT;
	std::weak_ptr<Car> frontCar_;
};

