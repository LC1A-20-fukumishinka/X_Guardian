#pragma once
#include "Vector3.h"
#include "carManager.h"

class XGuardian
{
private:
	//�����o�ϐ�
	Vector3 centerPos_;			//���W
	int XGuardianModelHandle_;		//�\���H�̎��_�{�̂̃��f���n���h��
	int XGuardiansStopModelHandle_;//�\���H�̎��_�̏�(��~)�̃��f���n���h��
	int XGuardiansStraightModelHandle_;//�\���H�̎��_�̏�(���i)�̃��f���n���h��
	int XGuardiansTurnRightModelHandle_;//�\���H�̎��_�̏�(�E��)�̃��f���n���h��
	int var;

public:
	MoveType inputSignal_;					//���͏��

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
};