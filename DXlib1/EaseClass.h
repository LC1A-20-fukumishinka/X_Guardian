#pragma once
class Easing
{
private:
	int timer;
	int maxTimer;
	float rate;

public://�v�Z�Ŏg���֐�
	static float easeInSine(const float rate);
	static float easeOutSine(const float rate);
	static float easeinOutSine(const float rate);
	static float easeInQuad(const float rate);
	static float easeOutQuad(const float rate);
	static float easeInOutQuad(const float rate);
	static float easeInCubic(const float rate);
	static float  easeOutCubic(const float rate);
	static float easeInOutCubic(const float rate);
	static float easeInQuart(const float rate);
	static float easeOutQuart(const float rate);
	static float easeInOutQuart(const float rate);
	static float easeInQuint(const float rate);
	static float easeOutQuint(const float rate);
	static float easeInOutQuint(const float rate);
	static float easeInExpo(const float rate);
	static float easeOutExpo(const float rate);
	static float easeInOutExpo(const float rate);
	static float easeInCirc(const float rate);
	static float easeOutCirc(const float rate);
	static float easeInOutCirc(const float rate);
	static float easeInBack(const float rate);
	static float easeOutBack(const float rate);
	static float easeInOutBack(const float rate);
	static float easeInElastic(const float rate);
	static float easeOutElastic(const float rate);
	static float easeInOutElastic(const float rate);
	static float easeInBounce(const float rate);
	static float easeOutBounce(const float rate);
	static float easeInOutBounce(const float rate);

public:
	const enum EaseMove
	{
		In,
		Out,
		InOut
	};

	const enum Type
	{
		Sine,
		Quad,
		Cubic,
		Quart,
		Quint,
		Expo,
		Circ,
		Back,
		Elastic,
		Bounce
	};
	Easing();

	//�C�[�W���O�v�Z
	static float EaseCalc(int InOrOut, int type, float rate);

	//�C�[�W���O�̑J��
	float Do(int InOrOut, int type);


	float Linear();
	//���l�̏�����
	void Init(int max = 1);

	//���l�̑J�ڂ��������Ă��邩���m�F����
	bool IsEnd();

	float Read();
	void Reset();

};