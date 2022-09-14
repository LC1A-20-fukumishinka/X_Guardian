#include "CarBlastParticle.h"
#include "DxLib.h"
#include "Car.h"
#include <algorithm>

using namespace std;
vector<Vector3> CarBlastParticle::nColors;
CarBlastParticle::CarBlastParticle()
{
	particles_.reserve(100);

	particles_.resize(100);

}

CarBlastParticle::~CarBlastParticle()
{
}

void CarBlastParticle::Init(ModelType type, Vector3 pos, int color)
{
	Vector3 particleColor = Vector3(1.0f, 1.0f, 1.0f);
	switch (type)
	{
	case ModelType::NORMAL:

		if (color >= nColors.size())
		{
			particleColor = nColors[0];
		}
		else
		{
			particleColor = nColors[color];
		}
		break;
	case ModelType::TRACK:
		if (color >= nColors.size())
		{
			particleColor = nColors[0];
		}
		else
		{
			particleColor = nColors[color];
		}

		//particleColor = Vector3(1.0f, 1.0f, 0.25f);
		break;
	default:
		break;
	}
	for (auto &e : particles_)
	{

		int overPower = rand() % 25;


		//float power = 1.0f;

		//if (overPower)
		//{
		//	power = rand() % 3;
		//}
		//else
		//{
		//	power = rand() % 10;
		//}

		float power = (static_cast<float>(rand()) / RAND_MAX);

		power = std::clamp(power, 0.5f, 1.0f);
		if (overPower)
		{
			power *= 5;
		}
		else
		{
			power *= 10;
		}
		e.Init(pos, RandBlastVector3(overPower), power, particleColor);
	}
}

void CarBlastParticle::Update()
{
	for (auto &e : particles_)
	{
		if (e.isAlive)
		{
		e.Update();
		}
	}
}

void CarBlastParticle::Finalize()
{
}

void CarBlastParticle::Draw()
{
	for (auto &e : particles_)
	{
		if (e.isAlive)
		{
			e.Draw();
		}
	}
}

void CarBlastParticle::InitializeColor()
{
	nColors.resize(5);
	float max = 255.0;
	nColors[0] = Vector3( 0.0f/max, 160.0f /max, 246.0f /max);
	nColors[1] = Vector3( 145.0f/max, 253.0f/max, 107.0f/max);
	nColors[2] = Vector3( 254.0f/max, 69.0f/max, 80.0f/max);
	nColors[3] = Vector3( 165.0f/max, 86.0f/max, 255.0f/max);
	nColors[4] = Vector3( 255.0f/max, 213.0f/max, 75.0f/max);
}

void particleObject::Init(Vector3 pos, Vector3 angle, float power, Vector3 color)
{
	speed = (angle * power);
	scale = 2.0f;
	this->pos = pos;
	this->color = color;
	isAlive = true;
	boundCount = 0;
}

void particleObject::Update()
{
	pos += speed;

	if (pos.y <= 0)
	{
		pos.y = 0.0f;
		if (boundCount <= 3)
		{
			boundCount++;
			speed *= (1.0f / (boundCount * 4));
			speed.y = abs(speed.y);
		}
		else
		{
			isAlive = false;
		}
	}


	float gravity = 0.1f;


	speed.y -= gravity;

}

void particleObject::Draw()
{
	Vector3 scaleV = Vector3(1.0f, 1.0f, 1.0f);

	scaleV *= scale;

	int Max = 255;
	DrawCube3D(pos - scaleV, pos + scaleV, GetColor((Max * color.x), (Max * color.y), (Max * color.z)), GetColor(Max, Max, Max), TRUE);
}

Vector3 RandBlastVector3(bool isNarrow)
{
	Vector3 ret;

	float xNarrow;
	float zNarrow;
	if (isNarrow)
	{
		xNarrow = 3.0f;
		zNarrow = 3.0f;
	}
	else
	{
		xNarrow = 1.0f;
		zNarrow = 1.0f;
	}
	ret.x = (rand() / xNarrow) - (RAND_MAX / (xNarrow * 2));
	ret.y = rand();
	ret.z = (rand() / zNarrow) - (RAND_MAX / (zNarrow * 2));
	return ret.normalaize();
}
