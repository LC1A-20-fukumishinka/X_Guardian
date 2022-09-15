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

bool Input::isPad(int pad)//引数　押すボタンの種類
{
	
	return pads.Buttons[pad];
}
//XINPUT_BUTTON_DPAD_UP		(0)	// デジタル方向ボタン上
//XINPUT_BUTTON_DPAD_DOWN		(1)	// デジタル方向ボタン下
//XINPUT_BUTTON_DPAD_LEFT		(2)	// デジタル方向ボタン左
//XINPUT_BUTTON_DPAD_RIGHT	(3)	// デジタル方向ボタン右
//XINPUT_BUTTON_START		(4)	// STARTボタン
//XINPUT_BUTTON_BACK		(5)	// BACKボタン
//XINPUT_BUTTON_LEFT_THUMB	(6)	// 左スティック押し込み
//XINPUT_BUTTON_RIGHT_THUMB	(7)	// 右スティック押し込み
//XINPUT_BUTTON_LEFT_SHOULDER	(8)	// LBボタン
//XINPUT_BUTTON_RIGHT_SHOULDER	(9)	// RBボタン
//XINPUT_BUTTON_A			(12)	// Aボタン
//XINPUT_BUTTON_B			(13)	// Bボタン
//XINPUT_BUTTON_X			(14)	// Xボタン
//XINPUT_BUTTON_Y			(15)	// Yボタン


bool Input::isPadTrigger(int pad)
{
	return (pads.Buttons[pad]) && !(oldpads.Buttons[pad]);
}

bool Input::isPadEnd(int pad)
{
	return !(pads.Buttons[pad]) && (oldpads.Buttons[pad]);
}

//引数　傾けるスティックの左右と向き
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

//XINPUT_THUMB_LEFTVERT		左スティックの縦方向
//XINPUT_THUMB_LEFTSIDE		左スティックの横方向
//XINPUT_THUMB_RIGHTVERT	右スティックの縦方向
//XINPUT_THUMB_RIGHTSIDE	右スティックの横方向

//＜使い方＞
//ただ入力してるとき移動させたい場合
//右に倒してるとき正、左に倒してるとき負の値が返るので戻り値の正負でif文を書けばよい

//入力の幅によって移動量を変えたい場合
//関数の戻り値を移動量にかけ、プレイヤーの座標に足せばいい
//プレイヤーの座標 += 関数の戻り値 * 移動量;

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

//XINPUT_TRIGGER_LEFT		左のトリガー
//XINPUT_TRIGGER_RIGHT		右のトリガー

//＜使い方＞
//基本的にisPadThumbと変わらない
//押してるとき正の値が返る


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
