#pragma once
#include "GameNum.h"
class GameInput
{
public:

	static void Update();

	static bool Straight(GameNum num);
	static bool TurnRight(GameNum num);

	static bool Pause(GameNum num);

	static bool Done(GameNum num);

	static bool ChangeMode();

	static bool Left();
	static bool Right();


private:

};

