#include "angryParticle.h"
#include "EaseClass.h"
#include "DxLib.h"
#include "Matrix4.h"

int angryParticle::angryParticleModelHandle = -1;
const int angryParticle::SpawnTimerMax = 30;
void angryParticleObject::Init(Vector3 pos, Vector3 angle, float power)
{
	this->pos = pos;
	this->speed = angle * power;
	alpha = 1.0f;
}

void angryParticleObject::Update()
{
	//���W�Ɉړ��ʂ����Z
	pos += speed;
	//�����x�������Ă���
	alpha -= (1.0f / 30.0f);

	isAlive = (alpha >= 0.0f);
}

void angryParticleObject::Draw(int handle)
{
	//�����������Ă������������[���񂮂�������
	float easerate = Easing::EaseCalc(Easing::Out, Easing::Type::Cubic, alpha);
	MV1SetOpacityRate(handle, easerate);

	//�A�t�B���ϊ�
	Matrix4 world = identity();
	world = scale(Vector3(0.01f, 0.01f, 0.01f));
	world *= translate(pos);

	//������s����Z�b�g�s�`��
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
				e.Init(pos, Vector3(xMove, 1.0f, 0.0f), 1.0f);
				e.isAlive = true;
				break;
			}
		}
	}
}
