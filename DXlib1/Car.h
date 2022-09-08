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
	void SetFrontCar(std::weak_ptr<Car> frontCar);

	void SetSignal(bool isStopSignal);
private:

	void CapsuleMove();

	void StraightMove();

	void RightTurnMove();

	//�J�v�Z����̓����蔻��g��Ȃ�����
std::unique_ptr<Capsule> colObject_;
//�ԗ��̐擪�����̍��W
Vector3 frontPos_;
//�ԗ��̕���
Vector3 angle_;

Vector3 centerPos_;
float carLength_;

float speed_;

bool isAlive_ = false;
bool isPlayer_ = true;

//�M�����Ԃ��ǂ���
bool isStopSignal_ = false;
MoveType type_ = MoveType::STRAIGHT;

std::weak_ptr<Car> frontCar_;
};

