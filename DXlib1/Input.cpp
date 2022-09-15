#include "Input.h"
#include "DxLib.h"

char Input::keys[256] = { 0 };
char Input::oldkeys[256] = { 0 };
int Input::mouses = 0;
int Input::oldmouses = 0;
XINPUT_STATE Input::pads;
XINPUT_STATE Input::oldpads;

void Input::Update()
{
	for( int i = 0; i < 256; i++ ) oldkeys[i] = keys[i];
	GetHitKeyStateAll(keys);
	oldmouses = mouses;
	mouses = GetMouseInput();
	oldpads = pads;
	GetJoypadXInputState(DX_INPUT_PAD1, &pads);
}

bool Input::isKey(int key)
{
	return keys[key];
}

bool  Input::isKeyTrigger(int key)
{
	return keys[key] && !oldkeys[key];
}

bool Input::isKeyEnd(int key)
{
	return !keys[key] && oldkeys[key];
}

bool Input::isMouse(int mouse)
{
	return (mouses & mouse);
}

bool Input::isMouseTrigger(int mouse)
{
	return (mouses & mouse) && !(oldmouses & mouse);
}

bool Input::isMouseEnd(int mouse)
{
	return !(mouses & mouse) && (oldmouses & mouse);
}

bool Input::isPad(int pad)//�����@�����{�^���̎��
{
	
	return pads.Buttons[pad];
}
//XINPUT_BUTTON_DPAD_UP		(0)	// �f�W�^�������{�^����
//XINPUT_BUTTON_DPAD_DOWN		(1)	// �f�W�^�������{�^����
//XINPUT_BUTTON_DPAD_LEFT		(2)	// �f�W�^�������{�^����
//XINPUT_BUTTON_DPAD_RIGHT	(3)	// �f�W�^�������{�^���E
//XINPUT_BUTTON_START		(4)	// START�{�^��
//XINPUT_BUTTON_BACK		(5)	// BACK�{�^��
//XINPUT_BUTTON_LEFT_THUMB	(6)	// ���X�e�B�b�N��������
//XINPUT_BUTTON_RIGHT_THUMB	(7)	// �E�X�e�B�b�N��������
//XINPUT_BUTTON_LEFT_SHOULDER	(8)	// LB�{�^��
//XINPUT_BUTTON_RIGHT_SHOULDER	(9)	// RB�{�^��
//XINPUT_BUTTON_A			(12)	// A�{�^��
//XINPUT_BUTTON_B			(13)	// B�{�^��
//XINPUT_BUTTON_X			(14)	// X�{�^��
//XINPUT_BUTTON_Y			(15)	// Y�{�^��


bool Input::isPadTrigger(int pad)
{
	return (pads.Buttons[pad]) && !(oldpads.Buttons[pad]);
}

bool Input::isPadEnd(int pad)
{
	return !(pads.Buttons[pad]) && (oldpads.Buttons[pad]);
}

//�����@�X����X�e�B�b�N�̍��E�ƌ���
float Input::isPadThumb(int pad)
{
	switch (pad)
	{
	case XINPUT_THUMB_LEFTVERT:
		return (float)pads.ThumbLY / (32767);
	case XINPUT_THUMB_LEFTSIDE:
		return (float)pads.ThumbLX / 32767;
	case XINPUT_THUMB_RIGHTVERT:
		return (float)pads.ThumbRY / 32767;
	case XINPUT_THUMB_RIGHTSIDE:
		return (float)pads.ThumbRX / 32767;
	default: return 0;
	}
}

//XINPUT_THUMB_LEFTVERT		���X�e�B�b�N�̏c����
//XINPUT_THUMB_LEFTSIDE		���X�e�B�b�N�̉�����
//XINPUT_THUMB_RIGHTVERT	�E�X�e�B�b�N�̏c����
//XINPUT_THUMB_RIGHTSIDE	�E�X�e�B�b�N�̉�����

//���g������
//�������͂��Ă�Ƃ��ړ����������ꍇ
//�E�ɓ|���Ă�Ƃ����A���ɓ|���Ă�Ƃ����̒l���Ԃ�̂Ŗ߂�l�̐�����if���������΂悢

//���͂̕��ɂ���Ĉړ��ʂ�ς������ꍇ
//�֐��̖߂�l���ړ��ʂɂ����A�v���C���[�̍��W�ɑ����΂���
//�v���C���[�̍��W += �֐��̖߂�l * �ړ���;

float Input::isPadThumbTrigger(int pad)
{
	switch (pad)
	{
	case XINPUT_THUMB_LEFTVERT:
		return ((float)pads.ThumbLY / 32767) && !((float)oldpads.ThumbLY / 32767);
	case XINPUT_THUMB_LEFTSIDE:
		return ((float)pads.ThumbLX / 32767) && !((float)oldpads.ThumbLX / 32767);
	case XINPUT_THUMB_RIGHTVERT:
		return ((float)pads.ThumbRY / 32767) && !((float)oldpads.ThumbRY / 32767);
	case XINPUT_THUMB_RIGHTSIDE:
		return ((float)pads.ThumbRX / 32767) && !((float)oldpads.ThumbRX / 32767);
	default: return 0;
	}
}

float Input::isPadThumbEnd(int pad)
{
	switch (pad)
	{
	case XINPUT_THUMB_LEFTVERT:
		return !((float)pads.ThumbLY / 32767) && ((float)oldpads.ThumbLY / 32767);
	case XINPUT_THUMB_LEFTSIDE:
		return !((float)pads.ThumbLX / 32767) && ((float)oldpads.ThumbLX / 32767);
	case XINPUT_THUMB_RIGHTVERT:
		return !((float)pads.ThumbRY / 32767) && ((float)oldpads.ThumbRY / 32767);
	case XINPUT_THUMB_RIGHTSIDE:
		return !((float)pads.ThumbRX / 32767) && ((float)oldpads.ThumbRX / 32767);
	default: return 0;
	}
}

float Input::isPadTri(int pad)
{
	switch (pad)
	{
	case XINPUT_TRIGGER_LEFT:
		return (float)pads.LeftTrigger / 255;
	case XINPUT_TRIGGER_RIGHT:
		return (float)pads.RightTrigger / 255;
	default: return 0;
	}
}

//XINPUT_TRIGGER_LEFT		���̃g���K�[
//XINPUT_TRIGGER_RIGHT		�E�̃g���K�[

//���g������
//��{�I��isPadThumb�ƕς��Ȃ�
//�����Ă�Ƃ����̒l���Ԃ�


float Input::isPadTriTrigger(int pad)
{
	switch (pad)
	{
	case XINPUT_TRIGGER_LEFT:
		return ((float)pads.LeftTrigger / 255) && !((float)oldpads.LeftTrigger / 255);
	case XINPUT_TRIGGER_RIGHT:
		return ((float)pads.RightTrigger / 255) && !((float)oldpads.RightTrigger / 255);
	default: return 0;
	}
}

float Input::isPadTriEnd(int pad)
{
	switch (pad)
	{
	case XINPUT_TRIGGER_LEFT:
		return !((float)pads.LeftTrigger / 255) && ((float)oldpads.LeftTrigger / 255);
	case XINPUT_TRIGGER_RIGHT:
		return !((float)pads.RightTrigger / 255) && ((float)oldpads.RightTrigger / 255);
	default: return 0;
	}
}
