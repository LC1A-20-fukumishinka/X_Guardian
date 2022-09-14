#pragma once
#include "Vector3.h"
#include "carManager.h"

class XGuardian
{
private:
	static const int MAX_SPINTIMER = 10;


	//�����o�ϐ�
	Vector3 centerPos_;			//���W
	int XGuardianModelHandle_;		//�\���H�̎��_�{�̂̃��f���n���h��
	int XGuardiansStopModelHandle_;//�\���H�̎��_�̏�(��~)�̃��f���n���h��
	int XGuardiansStraightModelHandle_;//�\���H�̎��_�̏�(���i)�̃��f���n���h��
	int XGuardiansTurnRightModelHandle_;//�\���H�̎��_�̏�(�E��)�̃��f���n���h��
	int var_;
	float angle_;
	int spinTimer_;
	bool isSpin_;
	float xGuardianAnimScale_;

public:
	MoveType inputSignal_;					//���͏��
	MoveType oldInputSignal_;				//1�t���[���O�̓��͏��

	void LoadModel();


	//�����o�֐�

	//�R���X�g���N�^
	XGuardian();
	//�f�X�g���N�^
	~XGuardian();
	//������
	void Init();
	//�X�V
	void Update(MoveType inputSignal);
	//�I��
	void Finalize();
	//�`��
	void Draw();

	//��]����
	void Spin();

	void ScaleAnimation(float easeRate);
};