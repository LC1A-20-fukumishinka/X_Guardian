#pragma once
#include"DxLib.h"

#define XINPUT_THUMB_LEFTVERT 0
#define XINPUT_THUMB_LEFTSIDE 1
#define XINPUT_THUMB_RIGHTVERT 2
#define XINPUT_THUMB_RIGHTSIDE 3

#define XINPUT_TRIGGER_LEFT 0
#define XINPUT_TRIGGER_RIGHT 1

class Input
{
private:
	//const int,const enumå^
	static char keys[];
	static char oldkeys[];
	static int mouses;
	static int oldmouses;
	static XINPUT_STATE pads;
	static XINPUT_STATE oldpads;
	//Input();

public:
	static void Update();
	static bool isKey(int key);
	static bool isKeyTrigger(int key);
	static bool isKeyEnd(int key);
	static bool isMouse(int mouse);
	static bool isMouseTrigger(int mouse);
	static bool isMouseEnd(int mouse);
	static bool isPad(int pad);
	static bool isPadTrigger(int pad);
	static bool isPadEnd(int pad);
	static float isPadThumb(int pad);
	static float isPadThumbTrigger(int pad);
	static float isPadThumbEnd(int pad);
	static float isPadTri(int pad);
	static float isPadTriTrigger(int pad);
	static float isPadTriEnd(int pad);
};