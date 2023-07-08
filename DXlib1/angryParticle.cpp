#include "angryParticle.h"
#include "EaseClass.h"
#include "Matrix4.h"
void angryParticleObject::Init(Vector3 pos, Vector3 angle, float power)
{
	this->pos = pos;
	this->speed = angle * power;
	alpha = 1.0f;
}

void angryParticleObject::Update()
{
	pos += speed;
	alpha -= (1.0f / 30.0f);
}

void angryParticleObject::Draw(int handle)
{
	float easerate = Easing::EaseCalc(Easing::Out, Easing::Type::Cubic, alpha);
	MV1SetOpacityRate(handle, easerate);

	Matrix4 world = identity();

	//world *= Matrix4::scale();
	//MV1SetMatrix(handle, );
}
