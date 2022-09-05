#pragma once
#include "DxLib.h"
#include "Vector3.h"

//MATRIX �^�� MATRIX4 �^�Ƃ��Ĉ��� ���݊����m��&�g���̂���
using Matrix4 = MATRIX;

//�P�ʍs������߂�
Matrix4 identity();

//�g��k���s��̐ݒ�
Matrix4 scale(const Vector3 &s);

//��]�s��̐ݒ�
Matrix4 rotationX(float angle);
Matrix4 rotationY(float angle);
Matrix4 rotationZ(float angle);

//���s�ړ�
Matrix4 translate(const Vector3 &t);

//���W�ϊ�(�x�N�g���ƍs��̊|���Z)
Vector3 transform(const Vector3 &v, const Matrix4 &m);

//������Z���̃I�[�o�[���[�h
Matrix4 &operator *=(Matrix4 &m1, const Matrix4 &m2);

//2�����Z�q�I�[�o�[���[�h
Matrix4 operator * (const Matrix4 &m1, const Matrix4 &m2);
Vector3 operator * (const Vector3 &v, const Matrix4 &m);