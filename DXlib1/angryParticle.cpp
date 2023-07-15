#include "angryParticle.h"
#include "EaseClass.h"
#include "DxLib.h"
#include "Matrix4.h"
#include <algorithm>
int angryParticle::angryParticleModelHandle = -1;
const int angryParticle::SpawnTimerMax = 30;
void angryParticleObject::Init(Vector3 pos, Vector3 angle, float power)
{
	this->pos = pos;
	pos.z -= -10.0f;
	this->speed = angle * power;
	alpha = 1.0f;
	objectScale = 0.0f;
}

void angryParticleObject::Update()
{
	//座標に移動量を加算
	pos += speed;

	speed.y *= 0.97f;

	speed.x *= 1.03f;
	//透明度を下げていく
	alpha -= (1.0f / 30.0f);

	isAlive = (alpha >= 0.0f);

	objectScale += (1.0f / 30.0f);
}

void angryParticleObject::Draw(int handle)
{
	//半透明化していく強さをいーじんぐをかける
	float easerate = Easing::EaseCalc(Easing::Out, Easing::Type::Cubic, alpha);
	MV1SetOpacityRate(handle, easerate);

	//アフィン変換
	Matrix4 world = identity();

	float scaleRate = Easing::EaseCalc(Easing::Out, Easing::Type::Cubic, objectScale);
	scaleRate *= 0.005f;
	world = scale(Vector3(scaleRate, scaleRate, scaleRate));
	world *= translate(pos);

	//作った行列をセット市描画
	MV1SetMatrix(handle, world);
	MV1DrawModel(handle);
}

void angryParticle::Init()
{
	if (angryParticleModelHandle < 0)
	{
		angryParticleModelHandle = MV1LoadModel("Resources/angry/angry.mv1");
	}
	particles_.resize(60);
}

void angryParticle::Update()
{
	for (auto& e : particles_)
	{
		if (e.isAlive)
		{
			e.Update();
		}
	}
}

void angryParticle::Finalize()
{
}

void angryParticle::Draw()
{
	for (auto& e : particles_)
	{
		if (e.isAlive)
		{
			e.Draw(angryParticleModelHandle);
		}
	}
}

void angryParticle::Spawn(const Vector3& pos)
{
	SpawnTimer++;

	if (SpawnTimer >= SpawnTimerMax)
	{
		SpawnTimer = 0;
		for (auto& e : particles_)
		{
			if (!e.isAlive)
			{
				float xMove = static_cast<float>(rand() % 100) / 100.0f;
				xMove -= 0.5f;
				xMove *= 0.5f;

				if (xMove <= 0.0f)
				{
					xMove = std::clamp(xMove, -0.5f, -0.1f);
				}
				else
				{
					xMove = std::clamp(xMove,  0.1f, 0.5f);
				}
				e.Init(pos, Vector3(xMove, 1.0f, 0.0f), 1.0f);
				e.isAlive = true;
				break;
			}
		}
	}
}
