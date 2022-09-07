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
	//�J�v�Z����̓����蔻��g��Ȃ�����
std::unique_ptr<Capsule> colObject_;
//�ԗ��̐擪�����̍��W
Vector3 frontPos_;
//�ԗ��̕���
Vector3 angle_;

Vector3 centerPos_;
float carLength_;


};

